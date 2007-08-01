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


namespace avs { namespace filters { namespace convert { namespace toyuy2 {



void FromYV12::ConvertFrame(VideoFrame const& source, VideoFrame& target) const
{

  CWindowPtr Y = source.ReadFrom('Y');
  CWindowPtr U = source.ReadFrom('U');
  CWindowPtr V = source.ReadFrom('V');
  WindowPtr dst = target.WriteTo('~');

  //Y plane
  for ( int y = dst.height; y-- > 0; Y.pad(), dst.pad() ) 
    for ( int x = dst.width / 4; x-- > 0; dst.to(4, 0), Y.to(2, 0) )
    {
      // copy in a macropixel
      dst[0] = Y[0];
      dst[2] = Y[1];
    }

  dst.to(0, -dst.height); //we return to the beginning of the plane.

  //U and V plane
  for ( int y = U.height; y-- > 0; U.pad(), V.pad(), dst.skipPad() )
    for ( int x = U.width; x-- > 0; U.to(1, 0), V.to(1, 0), dst.to(4, 0) )
    {
      // copy in a macropixel
      dst(1, 0) = U[0];
      dst(3, 0) = V[0];
      dst(1, 1) = U[0];
      dst(3, 1) = V[0];
    }
}



} } } } //namespace avs::filters::convert::toyuy2
