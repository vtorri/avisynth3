// Avisynth v3.0 alpha.  Copyright 2003-2005 David Pierre - Ben Rudiak-Gould et al.
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
#include "exporter.h"
#include "avistreaminfo.h"
#include "../../core/videoinfo.h"

#ifndef NOMINMAX
#define NOMINMAX          //prevents generation of min and max macros
#endif //NOMINMAX
#include <windows.h>
#include <vfw.h>

//assert include
#include <assert.h>


namespace avs { namespace export_ { namespace vfw {



AviStreamInfo::AviStreamInfo()
{
  memset(this, 0, sizeof(AviStreamInfo));
}


AviStreamInfo::AviStreamInfo(IAVIStream& stream)
{
  HRESULT hResult = stream.Info(reinterpret_cast<AVISTREAMINFOW *>(this), sizeof(AviStreamInfo));
  assert( hResult == S_OK );
}



AviStreamInfo::Video::Video(VideoInfo const& vi, Exporter const& exporter)
{
  fccType        = 'SDIV';
  fccHandler     = exporter.GetFourCCHandler();
  scale          = vi.GetFPSDenominator();    
  rate           = vi.GetFPSNumerator();
  length         = vi.GetFrameCount();
  suggestedBufferSize = exporter.GetBitmapSize(vi.GetDimension());
  quality        = 0xFFFFFFFF;     
  sampleSize     = suggestedBufferSize;

  frame.right    = vi.GetWidth();
  frame.bottom   = vi.GetHeight();      

  wcscpy(name, L"Avisynth video #1");  
}




AviStreamInfo::Audio::Audio(VideoInfo const& vi)
{
  fccType        = 'SDUA';
  scale          = vi.BytesPerAudioSample();
  rate           = vi.GetSampleRate() * scale;
  length         = static_cast<unsigned long>(vi.GetSampleCount());
  quality        = 0xFFFFFFFF;     
  sampleSize     = scale;
      
  wcscpy(name, L"Avisynth audio #1");  
}



} } } //namespace avs::export_::vfw

#endif //_WIN32
