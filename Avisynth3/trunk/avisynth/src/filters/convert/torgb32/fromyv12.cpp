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
      long scaledChromaToR = 32768 + VtoRCoeff * (V[0] - 128);
      long scaledChromaToG = 32768 - VtoGCoeff * (V[0] - 128) - UtoGCoeff *(U[0] - 128);
      long scaledChromaToB = 32768 + UtoBCoeff * (U[0] - 128);

      BYTE * ptr = dst.at(0, 0);
      
      //top-left pixel
      long scaledY = (Y[0] - 16) * YCoeff;
      ptr[0] = ( scaledY + scaledChromaToR ) >> 16;
      ptr[1] = ( scaledY + scaledChromaToG ) >> 16;
      ptr[2] = ( scaledY + scaledChromaToB ) >> 16;
      ptr[3] = 255;             

      //top-right pixel
      scaledY = (Y[1] - 16) * YCoeff;
      ptr[4] = ( scaledY + scaledChromaToR ) >> 16;
      ptr[5] = ( scaledY + scaledChromaToG ) >> 16;
      ptr[6] = ( scaledY + scaledChromaToB ) >> 16;
      ptr[7] = 255;             

      ptr = dst.at(0, 1);

      //bottom-left pixel
      scaledY = (Y(0, 1) - 16) * TCoeff;      
      ptr[0] = ( scaledY + scaledChromaToR ) >> 16;
      ptr[1] = ( scaledY + scaledChromaToG ) >> 16;
      ptr[2] = ( scaledY + scaledChromaToB ) >> 16;
      ptr[3] = 255;             

      //bottom-right pixel
      scaledY = (Y(1, 1) - 16) * YCoeff;
      ptr[4] = ( scaledY + scaledChromaToR ) >> 16;
      ptr[5] = ( scaledY + scaledChromaToG ) >> 16;
      ptr[6] = ( scaledY + scaledChromaToB ) >> 16;
      ptr[7] = 255;             
    }
}



} } } } //namespace avs::filters::convert::torgb32
