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
#include "avistreaminfo.h"
#include "../core/videoinfo.h"
#include "../core/colorspace.h"

//assert include
#include <assert.h>


namespace avs { namespace vfw {



AviStreamInfo::AviStreamInfo(VideoInfo const& vi, bool video)
{
  memset( static_cast<AVISTREAMINFOW *>(this), 0, sizeof(AVISTREAMINFOW) );

  if ( video )
  {
    int bmpSize = vi.GetColorSpace()->GetBitmapSize(vi.GetDimension());

    fccType        = streamtypeVIDEO;
    dwQuality      = DWORD(-1);     
    fccHandler     = vi.GetColorSpace()->GetFourCC();
    dwScale        = vi.GetFPSDenominator();    
    dwRate         = vi.GetFPSNumerator();
    dwLength       = vi.GetFrameCount();
    rcFrame.right  = vi.GetWidth();
    rcFrame.bottom = vi.GetHeight();      
    dwSampleSize   = bmpSize;
    dwSuggestedBufferSize = bmpSize; 

    wcscpy(szName, L"Avisynth video #1");  
  }
  else     //ie audio
  {
    int bps = vi.BytesPerAudioSample();

    fccType       = streamtypeAUDIO;
    dwQuality     = DWORD(-1);     
    fccHandler    = 0;
    dwScale       = bps;
    dwRate        = vi.GetSampleRate() * bps;
    dwLength      = static_cast<DWORD>(vi.GetSampleCount());
    dwSampleSize  = bps;
      
    wcscpy(szName, L"Avisynth audio #1");  
  }

}


void AviStreamInfo::SetLengthsTo(VideoInfo& vi) const
{
  if ( fccType == streamtypeVIDEO )
  {
    vi.SetFrameCount(dwLength);
    vi.SetFPS(dwRate, dwScale);
  }
  else
  {
    assert( fccType == streamtypeAUDIO );          //defend against weirdness

    vi.SetSampleCount(dwLength);
    vi.SetSampleRate(dwRate / dwScale);
  }
}


} } //namespace avs::vfw
