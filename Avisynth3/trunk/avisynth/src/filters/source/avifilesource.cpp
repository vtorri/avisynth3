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


//avisynth includes
#include "avifilesource.h"
#include "../../com/deleter.h"
#include "../../core/videoinfo.h"
#include "../../vfw/avistreaminfo.h"
#include "../../vfw/bitmapinfoheader.h"
#include "../../core/exception/noaudio.h"
#include "../../core/Exception/generic.h"
#include "../../core/exception/brokenfile.h"
#include "avi/vfwframedecompressor.h"

//windows includes
#include <windows.h>
#include <vfw.h>


namespace avs { namespace filters {


AviFileSource::Initializer::Initializer() { AVIFileInit(); }
AviFileSource::Initializer::~Initializer() { AVIFileExit(); }


AviFileSource::AviFileSource(std::string const& fileName, PEnvironment const& env)
  : AviSource( env )
{
  IAVIFile * paf = NULL;
  if ( AVIFileOpen(&paf, fileName.c_str(), OF_READ | OF_SHARE_DENY_WRITE, 0) != AVIERR_OK )     //if failure to open
    throw exception::BrokenFile(fileName);                       //exception
  aviFile_.reset(paf, com::Deleter<IAVIFile>());                 //wrap value in smart pointer

  video_.reset( GetStream(aviFile_, streamtypeVIDEO), com::Deleter<IAVIStream>() );

  long size = 0;
  video_->ReadFormat(0, NULL, &size);                            //get size needed for the format

  Block temp(size, false);                                       //allocates a block of that size
  HRESULT result = video_->ReadFormat(0, temp.get(), &size);     //and pass it to be filled
  assert( result == S_OK ); 

  //reinterpret buffer as a BitmapInfoHeader
  vfw::BitmapInfoHeader * bih = reinterpret_cast<vfw::BitmapInfoHeader *>(temp.get());
  //use it to create and set a frame decompressor, now it contains output format
  SetFrameDecompressor(avisource::VFWFrameDecompressor::Create(*this, *bih));

  vfw::AviStreamInfo asi;
  result = video_->Info(&asi, sizeof(vfw::AviStreamInfo));
  assert( result == S_OK );

  PVideoInfo vi = VideoInfo::Create();                           //creates an empty videoinfo
  vi->AddVideo(*bih->GetColorSpace(), bih->GetDimension(), 0);   //starts filling its video
  asi.SetLengthsTo(*vi);                                         //complete its video

  vi_ = vi;                                                      //save it as this'
}


BYTE * AviFileSource::GetAudio(BYTE * buffer, long long start, int count) const
{
  throw exception::NoAudio();
}


int AviFileSource::NearestKeyFrame(int n) const
{
  return video_->FindSample(n, FIND_KEY | FIND_PREV);
}


std::pair<OwnedBlock, long> AviFileSource::ReadVideo(int n) const
{
  std::pair<OwnedBlock, long> result = std::make_pair(OwnedBlock(GetEnvironment(), true), 0);

  video_->Read(n, 1, NULL, 0, &result.second, NULL);  //get needed size for the block

  if ( result.second != 0 )
  {
    result.first.reset( result.second + BufferWindow::Guard * 2, true );
  
    HRESULT hResult = video_->Read(n, 1, result.first.get() + BufferWindow::Guard, result.second, NULL, NULL);

    if ( hResult != AVIERR_OK )
      throw exception::Generic("Cannot read from source file");
  }

  return result;
}


IAVIStream * AviFileSource::GetStream(PAVIFile const& aviFile, unsigned long fccType)
{
  IAVIStream * result = NULL;

  //handles AVIERR_MEMORY error case, AVIERR_NODATA is naturally handled by returning NULL in that case
  if ( aviFile->GetStream(&result, fccType, 0) == AVIERR_MEMORY )
    throw std::bad_alloc(); 

  return result;
}


PClip AviFileSource::Create(std::string const& fileName, PEnvironment const& env) 
{ 
  return PClip( static_cast<Clip *>(new AviFileSource(fileName, env)) ); 
}


} } //namespace avs::filters
