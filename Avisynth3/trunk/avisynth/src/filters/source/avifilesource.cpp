// Avisynth v3.0 alpha.  Copyright 2005 David Pierre - Ben Rudiak-Gould et al.
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
#include "avifilesource.h"
#include "../../com/deleter.h"
#include "../../core/videoinfo.h"
#include "../../core/exception/brokenfile.h"

//windows includes
#ifndef NOMINMAX
#define NOMINMAX
#endif //NOMINMAX
#include <windows.h>
#include <vfw.h>


namespace avs { namespace filters {



AviFileSource::AviFileSource(std::string const& fileName, PEnvironment const& env)
  : clip::framemaker::Concrete( env )
{
  IAVIFile * paf = NULL;
  if ( AVIFileOpen(&paf, fileName.c_str(), OF_READ | OF_SHARE_DENY_WRITE, 0) != AVIERR_OK )     //if failure to open
    throw exception::BrokenFile(fileName);                       //exception
  aviFile_.reset(paf, com::Deleter<IAVIFile>());                 //wrap value in smart pointer

  PVideoInfo vi = VideoInfo::Create();                           //creates an empty videoinfo

  InitAudio( GetStream(aviFile_, streamtypeAUDIO), *vi);
  InitVideo( GetStream(aviFile_, streamtypeVIDEO), *vi);

  vi_ = vi;                                                      //save it as this' 
  
}


AviFileSource::PAVIStream AviFileSource::GetStream(PAVIFile const& aviFile, unsigned long fccType)
{
  IAVIStream * stream = NULL;
  //handles AVIERR_MEMORY error case, AVIERR_NODATA is naturally handled by returning NULL in that case
  if ( aviFile->GetStream(&stream, fccType, 0) == AVIERR_MEMORY )
    throw std::bad_alloc(); 

  return PAVIStream( stream, com::Deleter<IAVIStream>() );
}


boost::shared_ptr<void> AviFileSource::ReadFormat(PAVIStream const& aviStream)
{
  long size = 0;
  aviStream->ReadFormat(0, NULL, &size);              //fetch size of the format struct

  boost::shared_ptr<void> result( new BYTE[size] );   //allocates it into a shared_ptr

  HRESULT hResult = aviStream->ReadFormat(0, result.get(), &size);
  assert( hResult == S_OK ); 

  return result;
}


PClip AviFileSource::Create(std::string const& fileName, PEnvironment const& env) 
{ 
  return PClip( static_cast<Clip *>(new AviFileSource(fileName, env)) ); 
}


} } //namespace avs::filters

#endif //_WIN32
