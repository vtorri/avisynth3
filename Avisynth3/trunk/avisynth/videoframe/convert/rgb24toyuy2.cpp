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


//avisynth includes
#include "../yuy2videoframe.h"
#include "../rgb24videoframe.h"
#include "../../colorspace/yuy2.h"


namespace avs {


YUY2VideoFrame::YUY2VideoFrame(const RGB24VideoFrame& other)
  : InterleavedVideoFrame( CS::YUY2::instance(), other )
{

  CWindowPtr src = other.GetMain().GetReadPtr();
  WindowPtr dst = GetMain().GetWidth();

  const Dimension dim = GetDimension();

  src.to(0, dim.GetHeight() - 1);           //rgb is upside down

  const int cyb = int(0.114*219/255*65536+0.5);
  const int cyg = int(0.587*219/255*65536+0.5);
  const int cyr = int(0.299*219/255*65536+0.5);

  const int yuv_offset = dst->GetPitch() - dst->GetRowSize();
  const int rgb_offset = -src->GetPitch() - src->GetRowSize();
  const int rgb_inc = ((src_cs&VideoInfo::CS_BGR32)==VideoInfo::CS_BGR32) ? 4 : 3;

  for ( int y = dim.GetHeight(); y-- > 0; src.to(0, -1), dst.to(0, 1) ) 
  {
    BYTE * yuv = dst.ptr;
    const BYTE * rgb = src.ptr;

    for ( int x = dim.GetWidth()>>1; x-- > 0; yuv += 4, rgb += 3 ) 
    {
      // y1 and y2 can't overflow
      int y1 = ( cyb * rgb[0] + cyg * rgb[1] + cyr * rgb[2] + 0x108000 ) >> 16;
      int y2 = ( cyb * rgb[3] + cyg * rgb[4] + cyr * rgb[5] + 0x108000 ) >> 16;

      int scaled_y = ( y1 + y2 - 32 ) * int(255.0 / 219.0 * 32768 + 0.5);
      int b_y = ( rgb[0] + rgb[3] ) << 15 - scaled_y;
      int r_y = ( rgb[2] + rgb[5] ) << 15 - scaled_y;

      yuv[0] = y1;
      yuv[1] = ScaledPixelClip((b_y >> 10) * int(1 / 2.018 * 1024 + 0.5) + 0x800000);  // u
      yuv[2] = y2;
      yuv[3] = ScaledPixelClip((r_y >> 10) * int(1 / 1.596 * 1024 + 0.5) + 0x800000);  // v
    }
  }


}



}; //namespace avs

