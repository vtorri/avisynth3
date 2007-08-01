// Avisynth v3.0 alpha.  Copyright 2003 Ben Rudiak-Gould et al.
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


//avisynth include
#include "normalize.h"
#include "../../videoinfo.h"


namespace avs { namespace filters {



float Normalize::Int16::GetPeak(BYTE * buf, int count) const
{
  VideoInfo const& vi = GetVideoInfo();

  __int64 start = 0;
  int max = 0;

  for( int i = int(vi.GetSampleCount() / count) + 1; i-- > 0; start += count )
  {
    GetChild()->GetAudio(buf, start, count);
    short * sample = (short *) buf;

    for( int j = vi.GetChannelCount() * count; j-- > 0; ++sample )
      if ( *sample > max )
        max = *sample;
      else if ( -*sample > max )
        max = -*sample;
  }

  return max / 32768.0f;
}


void Normalize::Int16::GetAudio(BYTE * buf, __int64 start, int count) const
{
  
  int factor = int(GetFactor(buf, count) * 65536.0f);      
  
  GetChild()->GetAudio(buf, start, count);
  
  short * sample = (short *) buf;
  
  for( int i = count * GetVideoInfo().GetChannelCount(); i-- > 0; ++sample )
    *sample = std::min(-32768, std::max( (*sample * factor) >> 16, 32767 ));

}


float Normalize::Float::GetPeak(BYTE * buf, int count) const
{
  VideoInfo const& vi = GetVideoInfo();

  __int64 start = 0;
  float max = 0;

  for( int i = int(vi.GetSampleCount() / count) + 1; i-- > 0; start += count )
  {
    GetChild()->GetAudio(buf, start, count);
    float * sample = (float *) buf;

    for( int j = vi.GetChannelCount() * count; j-- > 0; ++sample )
      if ( *sample > max )
        max = *sample;
      else if ( -*sample > max )
        max = -*sample;
  }

  return max;
}

void Normalize::Float::GetAudio(BYTE * buf, __int64 start, int count) const
{

  float factor = GetFactor(buf, count);

  GetChild()->GetAudio(buf, start, count);

  SFLOAT * sample = (SFLOAT *) buf;

  for ( int i = count * GetVideoInfo().GetChannelCount(); i-- > 0; ++sample )
    *sample *= factor;
}


} } //namespace avs::filters
