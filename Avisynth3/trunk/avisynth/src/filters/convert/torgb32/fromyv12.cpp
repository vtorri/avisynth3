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
#include "fromyv12.h"
#include "../../../core/videoframe.h"


namespace avs { namespace filters { namespace convert { namespace torgb32 {



void FromYV12::ConvertFrame(VideoFrame const& source, VideoFrame& target) const
{
  CWindowPtr Y = source.ReadFrom('Y');
  CWindowPtr U = source.ReadFrom('U');
  CWindowPtr V = source.ReadFrom('V');
  WindowPtr dst = target.WriteTo('~');

  // Colour conversion from
  // http://www.poynton.com/notes/colour_and_gamma/ColorFAQ.html#RTFToC30
  //
  // YCbCr in Rec. 601 format
  // RGB values are in the range [0..255]
  //
  // [R]     1    [ 298.082       0       408.583 ]   ([ Y  ]   [  16 ])
  // [G] =  --- * [ 298.082   -100.291   -208.120 ] * ([ Cb ] - [ 128 ])
  // [B]    256   [ 298.082    516.411       0    ]   ([ Cr ]   [ 128 ])

  long coef_y   = long (298.082 * 256 + 0.5);
  long coef_r_v = long (408.583 * 256 + 0.5);
  long coef_g_v = long (208.120 * 256 + 0.5);
  long coef_g_u = long (100.291 * 256 + 0.5);
  long coef_b_u = long (516.411 * 256 + 0.5);

  for ( int y = U.height; y-- > 0; Y.skipPad(), U.pad(), V.pad(), dst.skipPad() )
    for ( int x = U.width; x-- > 0; Y.to(2, 0), U.to(1, 0), V.to(1, 0), dst.to(8, 0) )
    {
      unsigned char *ptr;

      long scaled_r_v = coef_r_v * (V[0] - 128);
      long scaled_g_v = coef_g_v * (V[0] - 128);
      long scaled_g_u = coef_g_u * (U[0] - 128);
      long scaled_b_u = coef_b_u * (U[0] - 128);

      // top-left pixel
      long scaled_y = (Y[0] - 16) * coef_y;

      ptr = dst.at(0, 0);
      ptr[0] = (scaled_y + scaled_r_v + 32768) >> 16;              //R
      ptr[1] = (scaled_y - scaled_g_u - scaled_g_v + 32768) >> 16; //G
      ptr[2] = (scaled_y + scaled_b_u + 32768) >> 16;              //B
      ptr[3] = 255;                                                //A

      // top-right pixel
      scaled_y = (Y(0, 1) - 16) * coef_y;

      ptr = dst.at(0, 1);
      ptr[0] = (scaled_y + scaled_r_v + 32768) >> 16;              //R
      ptr[1] = (scaled_y - scaled_g_u - scaled_g_v + 32768) >> 16; //G
      ptr[2] = (scaled_y + scaled_b_u + 32768) >> 16;              //B
      ptr[3] = 255;                                                //A

      // bottom-left pixel
      scaled_y = (Y(1, 0) - 16) * coef_y;

      ptr = dst.at(4, 0);
      ptr[0] = (scaled_y + scaled_r_v + 32768) >> 16;              //R
      ptr[1] = (scaled_y - scaled_g_u - scaled_g_v + 32768) >> 16; //G
      ptr[2] = (scaled_y + scaled_b_u + 32768) >> 16;              //B
      ptr[3] = 255;                                                //A

      // bottom-right pixel
      scaled_y = (Y(1, 1) - 16) * coef_y;

      ptr = dst.at(4, 1);
      ptr[0] = (scaled_y + scaled_r_v + 32768) >> 16;              //R
      ptr[1] = (scaled_y - scaled_g_u - scaled_g_v + 32768) >> 16; //G
      ptr[2] = (scaled_y + scaled_b_u + 32768) >> 16;              //B
      ptr[3] = 255;                                                //A
    }
}



} } } } //namespace avs::filters::convert::torgb32
