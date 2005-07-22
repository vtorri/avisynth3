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


//avisynth includes
#include "video.h"
#include "../exporter.h"
#include "../../core/videoinfo.h"

//assert include
#include <assert.h>


namespace avs { namespace vfw { namespace avistreaminfo {



Video::Video(IAVIStream& stream)
  : AviStreamInfo( stream )
{
  assert( fccType == streamtypeVIDEO );
}


Video::Video(VideoInfo const& vi, Exporter const& exporter)
{
  long bmpSize = exporter.GetBitmapSize(vi.GetDimension());

  fccType        = streamtypeVIDEO;
  dwQuality      = DWORD(-1);     
  fccHandler     = exporter.GetFourCCHandler();
  dwScale        = vi.GetFPSDenominator();    
  dwRate         = vi.GetFPSNumerator();
  dwLength       = vi.GetFrameCount();
  rcFrame.right  = vi.GetWidth();
  rcFrame.bottom = vi.GetHeight();      
  dwSampleSize   = bmpSize;
  dwSuggestedBufferSize = bmpSize; 

  wcscpy(szName, L"Avisynth video #1");  
}



void Video::Set(VideoInfo& vi) const
{
  vi.SetFrameCount(dwLength);
  vi.SetFPS(dwRate, dwScale);
}



} } } //namespace avs::vfw::avistreaminfo
