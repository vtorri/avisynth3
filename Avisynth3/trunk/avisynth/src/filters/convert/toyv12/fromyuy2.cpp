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


namespace avs { namespace filters { namespace convert { namespace toyv12 {



void FromYUY2::ConvertFrame(VideoFrame const& source, VideoFrame& target) const
{

  WindowPtr src = source.WriteTo('~');
  CWindowPtr Y = target.ReadFrom('Y');
  CWindowPtr U = target.ReadFrom('U');
  CWindowPtr V = target.ReadFrom('V');

  //Y plane
  for ( int y = src.height; y-- > 0; Y.pad(), src.pad() ) 
    for ( int x = src.width / 4; x-- > 0; src.to(4, 0), Y.to(2, 0) )
    {
      Y[0] = src[0];
      Y[1] = src[2];
    }

  src.to(0, -src.height); //we return to the beginning of the plane.

  //U and V plane
  for ( int y = U.height; y-- > 0; U.pad(), V.pad(), src.pad() )
    for ( int x = U.width; x-- > 0; U.to(1, 0), V.to(1, 0), src.to(4, 0) )
    {
      // copy in a macropixel
      U[0] = src[1];
      V[0] = src[3];
    }
}



} } } } //namespace avs::filters::convert::toyv12
