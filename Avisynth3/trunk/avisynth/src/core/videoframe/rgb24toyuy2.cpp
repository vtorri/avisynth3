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
#include "rgb.h"
#include "yuy2.h"
#include "../saturate.h"


namespace avs { namespace vframe {



YUY2::YUY2(RGB24 const& source)
  : interleaved<2, 1>( ColorSpace::yuy2(), source )
{

  CWindowPtr src = source.GetMain().Read();
  WindowPtr dst = GetMain().Write();

  src.to(0, src.height - 1);           //rgb is upside down

  static int const cyb = int(0.114 * 219 / 255 * 65536 + 0.5);
  static int const cyg = int(0.587 * 219 / 255 * 65536 + 0.5);
  static int const cyr = int(0.299 * 219 / 255 * 65536 + 0.5);


  for ( int y = dst.height; y-- > 0; src.to(0, -1), dst.to(0, 1) ) 
  {
    BYTE * yuv = dst.ptr;
    BYTE const * rgb = src.ptr;

    for ( int x = dst.width / 2; x-- > 0; yuv += 4, rgb += 6 ) 
    {
      // y1 and y2 can't overflow
      int y1 = ( cyb * rgb[0] + cyg * rgb[1] + cyr * rgb[2] + 0x108000 ) >> 16;
      int y2 = ( cyb * rgb[3] + cyg * rgb[4] + cyr * rgb[5] + 0x108000 ) >> 16;

      int scaled_y = ( y1 + y2 - 32 ) * int(255.0 / 219.0 * 32768 + 0.5);
      int b_y = (( rgb[0] + rgb[3] ) << 15) - scaled_y;
      int r_y = (( rgb[2] + rgb[5] ) << 15) - scaled_y;

      yuv[0] = y1;
      yuv[1] = saturate<BYTE>( ((b_y >> 10) * int(1 / 2.018 * 1024 + 0.5) + 0x800000) >> 16 );  // u
      yuv[2] = y2;
      yuv[3] = saturate<BYTE>( ((r_y >> 10) * int(1 / 1.596 * 1024 + 0.5) + 0x800000) >> 16 );  // v
    }
  }

}



} } //namespace avs::vframe

