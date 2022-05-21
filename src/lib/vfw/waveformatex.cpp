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
#include "waveformatex.h"
#include "../core/videoinfo.h"

//normally in Mmreg.h but mingw doesn't has it :p
#define  WAVE_FORMAT_MPEGLAYER3   0x0055   //ISO/MPEG Layer3 Format Tag 


namespace avs { namespace vfw {



WaveFormatEx::WaveFormatEx(VideoInfo const& vi)
{
  wFormatTag      = WAVE_FORMAT_PCM;
  nChannels       = vi.GetChannelCount();
  nSamplesPerSec  = vi.GetSampleRate();
  nBlockAlign     = vi.BytesPerAudioSample();
  nAvgBytesPerSec = nSamplesPerSec * nBlockAlign;
  wBitsPerSample  = vi.BytesPerChannelSample() * 8;
  cbSize          = 0; 
}


bool WaveFormatEx::IsVBR() const 
{ 
  //mp3 vbr hackishly use non-1 block align 
  if ( wFormatTag == WAVE_FORMAT_MPEGLAYER3 )
    return nBlockAlign != 1;

  //other cases rely on wBitsPerSample (hoping it's correclty set)
  return wBitsPerSample == 0; 
}


} } //namespace avs::vfw



#endif //_WIN32
