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

//avisynth includes
#include "rawaudio.h"
#include "../avifilesource.h"                //for AviFileSource::ReadFormat
#include "../../../core/videoinfo.h"
#include "../../../vfw/waveformatex.h"
#include "../../../vfw/avistreaminfo.h"
#include "../audio/acmaudiodecompressor.h"
#include "../../../core/exception/generic.h"

//windows includes
#ifndef NOMINMAX
#define NOMINMAX
#endif //NOMINMAX
#include <windows.h>
#include <vfw.h>
#ifndef AVISTREAMREAD_CONVENIENT        
#define AVISTREAMREAD_CONVENIENT -1
#endif //AVISTREAMREAD_CONVENIENT

//assert include
#include <assert.h>


namespace avs { namespace filters { namespace source { namespace avifile {



long RawAudio::ReadAudio(BYTE * buffer, long long start, long& size) const
{
  long result = 0;                          //accumulates samples read (function result)
  long bufferSize = size;                   //accumulates bytes read
  size = 0;                                 //0 bytes read (so far)
  long bps = GetVideoInfo()->BytesPerAudioSample();
  long lStart = static_cast<long>(start);

  while( true )
  {
    long samplesRead = 0;
    long bytesRead = 0;

    //reads data from stream
    HRESULT hResult = audio_->Read(lStart, AVISTREAMREAD_CONVENIENT, buffer, bufferSize, &bytesRead, &samplesRead);

    if ( hResult == AVIERR_FILEREAD )
      throw exception::Generic("Cannot read from source file");    
    if (  bytesRead == 0 )                  //exits while when nothing more was rea
      break;
    assert( hResult == AVIERR_OK );

    buffer += bytesRead;                    //move ptr after data read
    bufferSize -= bytesRead;                //bytesRead less space available in buffer
    size += bytesRead;

    result += samplesRead;                  //samplesRead more samples read
    lStart += samplesRead;                  //next sample to read
  }

  return result;                            //return total samples read
}



void RawAudio::InitAudio(PAVIStream const& audio, VideoInfo& vi)
{
  if ( audio )
  {
    //get wav format header from stream
    vfw::PWaveFormatEx wfe = boost::static_pointer_cast<vfw::WaveFormatEx>(AviFileSource::ReadFormat(audio));
    
    if ( ! wfe->IsVBR() )        //checks it's CBR  (vfw IAVIStream seems unable to output vbr data)
    {
      audio_ = audio;

      //sets audio decompressor (and gets output format in wfe)
      SetAudioDecompressor( ACMAudioDecompressor::Create(*this, wfe) );
      vfw::AviStreamInfo asi(*audio);

      assert( wfe->wBitsPerSample == 8 || wfe->wBitsPerSample == 16 );
      vi.AddAudio( wfe->wBitsPerSample == 8 ? SAMPLE_INT8 : SAMPLE_INT16, wfe->nSamplesPerSec, 0, wfe->nChannels );                 
      asi.SetLengthsTo(vi);
    }
  }
}


} } } } //namespace avs::filters::source::avifile

#endif //_WIN32
