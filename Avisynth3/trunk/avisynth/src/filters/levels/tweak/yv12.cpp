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
#include "yv12.h"
#include "../../../core/videoframe.h"
#include "../../../core/cow_shared_ptr.h"
#include "../../../core/utility/saturate.h"


namespace avs { namespace filters { namespace tweak {



CPVideoFrame YV12::MakeFrame(PVideoFrame const& source) const
{

  WindowPtr Y = source->WriteTo(PLANAR_Y);
  WindowPtr U = source->WriteTo(PLANAR_U);
  WindowPtr V = source->WriteTo(PLANAR_V);


  for( int y = Y.height; y-- > 0; Y.pad() )
    for( int x = Y.width; x-- > 0; ++Y.ptr )
    {
      // brightness and contrast 
      int yy = *Y.ptr - 16;
      yy = (Cont * yy) >> 9;
      yy += Bright_p16;
      *Y.ptr = saturate<BYTE, 15, 235>(yy);
    }


  for( int y = U.height; y-- > 0; U.pad(), V.pad() )
    for ( int x = U.width; x-- > 0; ++U.ptr, ++V.ptr )
    {
      // hue and saturation 
			int u = *U.ptr - 128;
			int	v = *V.ptr - 128;

			int	ux = (+ u * Cos + v * Sin) >> 12;
      int vx = (- u * Sin + v * Cos) >> 12;
				
			u = ((ux * Sat) >> 9) + 128;
			v = ((vx * Sat) >> 9) + 128;
			
      *U.ptr = saturate<BYTE, 16, 240>(u);
      *V.ptr = saturate<BYTE, 16, 240>(v);				      
    }

  return source;
}



} } } //namespace avs::filters::tweak
