// Avisynth v3.0 alpha.  Copyright 2004 David Pierre - Ben Rudiak-Gould et al.
// http://www.avisynth.org

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA, or visit
// http://www.gnu.org/copyleft/gpl.html .
//
// Linking Avisynth statically or dynamically with other modules is making a
// combined work based on Avisynth.  Thus, the terms and conditions of the GNU
// General Public License cover the whole combination.


#ifdef _WIN32

//avisynth include
#include "acmaudiodecompressor.h"
#include "trivialaudiodecompressor.h"
#include "../../../vfw/waveformatex.h"
#include "../../../core/videoinfo.h"
#include "../../../core/exception/generic.h"

//stl include
#include <algorithm>     //for std::min


namespace avs { namespace filters { namespace source {


namespace {


//stream deleter to use with boost::shared_ptr
struct ACMSTreamCloser
{
  void operator()(HACMSTREAM stream) const { acmStreamClose(stream, 0); }
};


} //namespace anonymous



ACMAudioDecompressor::ACMAudioDecompressor(RawAudio const& src, vfw::PWaveFormatEx& wfe)
  : src_( src )
  , current_( 0 )
  , nextBlock_( 0 )
  , blockAlign_( wfe->nBlockAlign )
  , samplesPerSec_( wfe->nSamplesPerSec )
  , avgBytesPerSec_( wfe->nAvgBytesPerSec )
{

  DWORD maxSizeFormat = 0;
  //fetchs max size of a WAVEFORMATEX object (into maxSizeFormat)
  MMRESULT mmResult = acmMetrics(NULL, ACM_METRIC_MAX_SIZE_FORMAT, &maxSizeFormat);
  assert( mmResult == 0 );                    //the above can't fail (or can it ?)

  //creates a WaveFormatEx of that size
  boost::shared_ptr<void> temp( new BYTE[maxSizeFormat] );
  vfw::PWaveFormatEx output = boost::static_pointer_cast<vfw::WaveFormatEx>(temp);

  output->wFormatTag = WAVE_FORMAT_PCM;       //and inits it with PCM format

  //ask ACM for a PCM format suggestion
  if ( acmFormatSuggest(NULL, wfe.get(), output.get(), maxSizeFormat, ACM_FORMATSUGGESTF_WFORMATTAG) != 0 )
    throw exception::Generic("ACM failed to suggest a compatible PCM format");
  
  HACMSTREAM__ * hACMStream = NULL;
  if ( acmStreamOpen(&hACMStream, NULL, wfe.get(), output.get(), NULL, 0, 0, ACM_STREAMOPENF_NONREALTIME) != 0 )
    throw exception::Generic("Error initializing audio stream decompression");
 
  hACMStream_.reset( hACMStream, ACMSTreamCloser() );

  DWORD outputBufferSize = 0;
  //gets size of output buffer for given size of input buffer
	if ( acmStreamSize(hACMStream, inputBufferSize, &outputBufferSize, ACM_STREAMSIZEF_SOURCE) != 0 )
		throw exception::Generic("Error initializing audio stream output size");

  //allocates memory for both buffers
  inputBuffer_.reset( new BYTE[inputBufferSize] );
  outputBuffer_.reset( new BYTE[outputBufferSize] );
		
  memset(&ash_, 0, sizeof(ACMSTREAMHEADER));

	ash_.cbStruct		  = sizeof(ACMSTREAMHEADER);
  ash_.pbSrc			  = inputBuffer_.get();
	ash_.cbSrcLength	= inputBufferSize;
	ash_.pbDst			  = outputBuffer_.get();
	ash_.cbDstLength	= outputBufferSize;

	if ( acmStreamPrepareHeader(hACMStream, &ash_, 0) != 0)
		throw exception::Generic("Error preparing audio decompression buffers");

	ash_.cbSrcLength = 0;
	ash_.cbDstLengthUsed = 0;

  wfe = output;              //finally reports output waveformat to caller
}


ACMAudioDecompressor::~ACMAudioDecompressor()
{
  ash_.cbSrcLength = inputBufferSize;                     //restores value it had during prepare
	ash_.pbDst			 = outputBuffer_.get();                 //idem
	acmStreamUnprepareHeader(hACMStream_.get(), &ash_, 0);  //unprepare conversion stream
}


long ACMAudioDecompressor::operator()(BYTE *& buffer, long long start, long count) const
{
  int bps = src_.GetVideoInfo()->BytesPerAudioSample();   //bytes per audio sample

  long long advance = start * bps - current_;  
  if ( 0 <= advance && advance <= ash_.cbDstLengthUsed )  //if the data start is in the output buffer
    SkipFromBuffer( static_cast<long>(advance) );         //skip to that point
  else
  {                                                       //else we must seek :(
    //round to nearest audio block
    long long block = (start * avgBytesPerSec_) / (static_cast<long long>(blockAlign_) * samplesPerSec_);
    long long roundedStart = (block * blockAlign_ * samplesPerSec_) / avgBytesPerSec_;

    nextBlock_ = block;                                   //set rounded as new position
    current_ = roundedStart * bps;

    DecompressNext();                                     //decompress data there

    Skip(static_cast<long>(start - roundedStart));        //skip to reach start
    //NB: it may skip less than that if it reachs end of stream
    //but then it will correctly read nothing after, so it's not a pb
  }

  return Read(buffer, count * bps) / bps;                 //just read and return samples read (ie bytes read / bps )
}



void ACMAudioDecompressor::DecompressNext() const
{
  long size = inputBufferSize - ash_.cbSrcLength;   //space available in input buffer
  nextBlock_ += src_.ReadAudio(ash_.pbSrc + ash_.cbSrcLength, nextBlock_, size);

  ash_.cbSrcLength += size;                         //update size of input data

  ash_.cbDstLengthUsed = 0;                         //flush output buffer
  ash_.pbDst = outputBuffer_.get();

  if ( ash_.cbSrcLength != 0 )                      //if there is something to convert
  {
    ash_.cbSrcLengthUsed = 0;                       

  	if ( acmStreamConvert(hACMStream_.get(), &ash_, ACM_STREAMCONVERTF_BLOCKALIGN) != 0 )
      throw exception::Generic("ACM failed to decompress audio");

    long left = ash_.cbSrcLength - ash_.cbSrcLengthUsed;             //unused data in the input buffer
    if ( left > 0 )                                                  //if there is some                                
      memmove(ash_.pbSrc, ash_.pbSrc + ash_.cbSrcLengthUsed, left);  //copy it down

    ash_.cbSrcLength = left;                        //update data size in input buffer
  }

}


long ACMAudioDecompressor::Read(BYTE *& buffer, long size) const
{
  long read = 0;

  if ( ash_.cbDstLengthUsed != 0 )                   //there is data left in output buffer
    read += ReadFromBuffer(buffer, size);            //get it (updating buffer and size)

  while ( size > 0 )                                 //while there is data left to fill
  {
    DecompressNext();                                //decompress more data

    if ( ash_.cbDstLengthUsed == 0 )                 //if nothing got decompressed
      break;                                         //there is no more, we stop here

    read += ReadFromBuffer(buffer, size);            //else we read what we can
  }

  return read;
}


long ACMAudioDecompressor::ReadFromBuffer(BYTE *& buffer, long& size) const
{
  BYTE * src = ash_.pbDst;                              //save src ptr (will be modified by skip call)
  long read = SkipFromBuffer(size);                     //jump over the data we want to read and get the real size available

  memcpy(buffer, src, read);                            //copy data

  size -= read;                                         //count down data left to fill
  buffer += read;                                       //update buffer

  return read;                                          //return size of data read
}


void ACMAudioDecompressor::Skip(long size) const
{
  if ( ash_.cbDstLengthUsed != 0 )           //there is data left in output buffer
    size -= SkipFromBuffer(size);            //skip what we can

  while ( size > 0 )                         //while there is more to skip
  {
    DecompressNext();                        //decompress more data

    if ( ash_.cbDstLengthUsed == 0 )         //if nothing got decompressed
      break;                                 //there is no more, we stop here

    size -= SkipFromBuffer(size);            //else we skip what we can
  }
}



long ACMAudioDecompressor::SkipFromBuffer(long size) const
{
  long skip = std::min(size, long(ash_.cbDstLengthUsed)); //available to skip

  current_ += skip;                                       //update position

  ash_.pbDst += skip;                                     //update output buffer ptr
  ash_.cbDstLengthUsed -= skip;                           //and output buffer size

  return skip;                                            //report data skipped
}



AudioDecompressor * ACMAudioDecompressor::Create(RawAudio const& src, vfw::PWaveFormatEx& wfe)
{
  if ( wfe->wFormatTag == WAVE_FORMAT_PCM )     //if it's already PCM
    return new TrivialAudioDecompressor(src);   //trivial decompression

  return new ACMAudioDecompressor(src, wfe);    //else use acm decompressor
}



} } } //namespace avs::filters::source

#endif //_WIN32
