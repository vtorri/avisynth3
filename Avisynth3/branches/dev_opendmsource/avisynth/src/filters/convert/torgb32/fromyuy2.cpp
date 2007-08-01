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
#include "fromyuy2.h"
#include "../../../core/videoframe.h"
#include "../../../core/utility/saturate.h"


namespace avs { namespace filters { namespace convert { namespace torgb32 {



void FromYUY2::ConvertFrame(VideoFrame const& source, VideoFrame& target) const
{
  ConvertFrame( source.ReadFrom('~'), target.WriteTo('~') );
}


void FromYUY2::ConvertFrame(CWindowPtr src, WindowPtr dst)
{
  using namespace utility;

  // Colour conversion from
  // http://www.poynton.com/notes/colour_and_gamma/ColorFAQ.html#RTFToC30
  //
  // YCbCr in Rec. 601 format
  // RGB values are in the range [0..255]
  //
  // [R]     1    [ 298.082       0       408.583 ]   ([ Y  ]   [  16 ])
  // [G] =  --- * [ 298.082   -100.291   -208.120 ] * ([ Cb ] - [ 128 ])
  // [B]    256   [ 298.082    516.411       0    ]   ([ Cr ]   [ 128 ])

  long YCoeff    = long (298.082 * 256 + 0.5);
  long VtoRCoeff = long (408.583 * 256 + 0.5);
  long VtoGCoeff = long (208.120 * 256 + 0.5);
  long UtoGCoeff = long (100.291 * 256 + 0.5);
  long UtoBCoeff = long (516.411 * 256 + 0.5);

  for ( int y = src.height; y-- > 0; src.pad(), dst.pad() )
    for ( int x = src.width / 4; x-- > 0; src.to(4, 0), dst.to(8, 0) )
    {
      long scaledChromaToR = 32768 + VtoRCoeff * (src[3] - 128);
      long scaledChromaToG = 32768 - VtoGCoeff * (src[3] - 128) - UtoGCoeff *(src[1] - 128);
      long scaledChromaToB = 32768 + UtoBCoeff * (src[1] - 128);

      BYTE * ptr = dst.at(0, 0);

      //left pixel
      long scaledY = (src[0] - 16) * YCoeff;
      ptr[0] = saturate<BYTE, 0, 255>(( scaledY + scaledChromaToR ) >> 16);
      ptr[1] = saturate<BYTE, 0, 255>(( scaledY + scaledChromaToG ) >> 16);
      ptr[2] = saturate<BYTE, 0, 255>(( scaledY + scaledChromaToB ) >> 16);
      ptr[3] = 255;

      //right pixel
      scaledY = (src[2] - 16) * YCoeff;
      ptr[4] = saturate<BYTE, 0, 255>(( scaledY + scaledChromaToR ) >> 16);
      ptr[5] = saturate<BYTE, 0, 255>(( scaledY + scaledChromaToG ) >> 16);
      ptr[6] = saturate<BYTE, 0, 255>(( scaledY + scaledChromaToB ) >> 16);
      ptr[7] = 255;
    }
}



} } } } //namespace avs::filters::convert::torgb32
