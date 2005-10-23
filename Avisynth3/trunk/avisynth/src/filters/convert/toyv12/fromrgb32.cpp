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
#include "fromrgb32.h"
#include "../../../core/videoframe.h"


namespace avs { namespace filters { namespace convert { namespace toyv12 {



void FromRGB32::ConvertFrame(VideoFrame const& source, VideoFrame& target) const
{
  ConvertFrame( source.ReadFrom('~'), target.WriteTo('Y'), target.WriteTo('U'), target.WriteTo('V') );
}

void FromRGB32::ConvertFrame(CWindowPtr src, WindowPtr Y, WindowPtr U, WindowPtr V) const
{

  // Colour conversion from
  // http://www.poynton.com/notes/colour_and_gamma/ColorFAQ.html#RTFToC30
  //
  // YCbCr in Rec. 601 format
  // RGB values are in the range [0..255]
  //
  // [ Y  ]   [  16 ]    1    [  65.738    129.057    25.064  ]   [ R ]
  // [ Cb ] = [ 128 ] + --- * [ -37.945    -74.494    112.439 ] * [ G ]
  // [ Cr ]   [ 128 ]   256   [ 112.439    -94.154    -18.285 ]   [ B ]

  long RtoYCoeff = long (65.738  * 256 + 0.5);
  long GtoYCoeff = long (129.057 * 256 + 0.5);
  long BtoYCoeff = long (25.064  * 256 + 0.5);

  long RtoUCoeff = long (-37.945 * 256 + 0.5);
  long GtoUCoeff = long (-74.494 * 256 + 0.5);
  long BtoUCoeff = long (112.439 * 256 + 0.5);

  long RtoVCoeff = long (112.439 * 256 + 0.5);
  long GtoVCoeff = long (-94.154 * 256 + 0.5);
  long BtoVCoeff = long (-18.285 * 256 + 0.5);

  BYTE U00, U01, U10, U11;
  BYTE V00, V01, V10, V11;

  //Y plane
  for ( int y = src.height; y-- > 0; Y.pad(), src.pad() )
    for ( int x = src.width / 4; x-- > 0; src.to(4, 0), Y.to(1, 0) )
    {
      // No need to saturate between 16 and 235
      Y[0] = 16 + ((32768 + RtoYCoeff * src[0] + GtoYCoeff * src[1] + BtoYCoeff * src[2]) >> 16);
    }

  src.to(0, -src.height); //we return to the beginning of the plane.

  //U and V planes
  for ( int y = U.height; y-- > 0; U.pad(), V.pad(), src.skipPad() )
    for ( int x = U.width; x-- > 0; U.to(1, 0), V.to(1, 0), src.to(8, 0) )
    {
      // No need to saturate between 16 and 240
      U00 = 128 + ((32768 + RtoUCoeff * src[0] + GtoUCoeff * src[1] + BtoUCoeff * src[2]) >> 16);
      U01 = 128 + ((32768 + RtoUCoeff * src[4] + GtoUCoeff * src[5] + BtoUCoeff * src[6]) >> 16);
      U10 = 128 + ((32768 + RtoUCoeff * src(0, 1) + GtoUCoeff * src(1, 1) + BtoUCoeff * src(2, 1)) >> 16);
      U11 = 128 + ((32768 + RtoUCoeff * src(4, 1) + GtoUCoeff * src(5, 1) + BtoUCoeff * src(6, 1)) >> 16);
      U[0] = 2 + ((U00 + U01 + U10 + U11) >> 2);

      V00 = 128 + ((32768 + RtoVCoeff * src[0] + GtoVCoeff * src[1] + BtoVCoeff * src[2]) >> 16);
      V01 = 128 + ((32768 + RtoVCoeff * src[4] + GtoVCoeff * src[5] + BtoVCoeff * src[6]) >> 16);
      V10 = 128 + ((32768 + RtoVCoeff * src(0, 1) + GtoVCoeff * src(1, 1) + BtoVCoeff * src(2, 1)) >> 16);
      V11 = 128 + ((32768 + RtoVCoeff * src(4, 1) + GtoVCoeff * src(5, 1) + BtoVCoeff * src(6, 1)) >> 16);
      V[0] = 2 + ((V00 + V01 + V10 + V11) >> 2);
    }
}



} } } } //namespace avs::filters::convert::toyv12
