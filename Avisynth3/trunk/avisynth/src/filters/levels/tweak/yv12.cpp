// Avisynth v3.0 alpha.  Copyright 2004 Ben Rudiak-Gould et al.
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


namespace avs { namespace filters { namespace tweak {



CPVideoFrame YV12::MakeFrame(CPVideoFrame const& source) const
{

  PVideoFrame frame = source;

  WindowPtr Y = frame->WriteTo(PLANAR_Y);
  WindowPtr U = frame->WriteTo(PLANAR_U);
  WindowPtr V = frame->WriteTo(PLANAR_V);


  for( int y = Y.height; y-- > 0; Y.to(0, 1) )
  {
    BYTE * ptr = Y.ptr;

    for( int x = Y.width; x-- > 0; ++ptr )
    {
      // brightness and contrast 
      int y = *ptr - 16;
      y = (Cont * y) >> 9;
      y += Bright_p16;
      *ptr = std::min( std::max(15, y), 235);
    }
  }


  for( y = U.height; y-- > 0; U.to(0, 1), V.to(0, 1) )
  {
    BYTE * pu = U.ptr;
    BYTE * pv = V.ptr;

    for ( int x = U.width; x-- > 0; ++pu, ++pv )
    {
      // hue and saturation 
			int u = *pu - 128;
			int	v = *pv - 128;

			int	ux = (+ u * Cos + v * Sin) >> 12;
      int vx = (- u * Sin + v * Cos) >> 12;
				
			u = ((ux * Sat) >> 9) + 128;
			v = ((vx * Sat) >> 9) + 128;
			
      *pu = std::min( std::max(16, u), 240 );
      *pv = std::min( std::max(16, v), 240 );				      
    }
  }

  return frame;
}



} } } //namespace avs::filters::tweak
