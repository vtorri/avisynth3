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
#include "yuy2.h"
#include "../../../core/videoframe.h"
#include "../../../core/cow_shared_ptr.h"


namespace avs { namespace filters { namespace tweak {



CPVideoFrame YUY2::MakeFrame(CPVideoFrame const& source) const
{

  PVideoFrame frame = source;
  WindowPtr wp = frame->WriteTo(NOT_PLANAR);

    
  for ( int y = wp.height; y-- > 0; wp.to(0, 1) )		
  {
    BYTE * ptr = wp.ptr;

    for ( int x = wp.width >> 2; x-- > 0; ptr += 4 )			
    {				
      // brightness and contrast 
	  	int	y1 = ptr[0] - 16;
		  int y2 = ptr[2] - 16;
				
      y1 = (Cont * y1) >> 9;
  		y2 = (Cont * y2) >> 9;
			
      ptr[0] = std::min(std::max(0, y1 + Bright_p16), 255);
		  ptr[2] = std::min(std::max(0, y2 + Bright_p16), 255);


 			// hue and saturation 
  		int	u = ptr[1] - 128;
	  	int	v = ptr[3] - 128;

		  int	ux = (+ u * Cos + v * Sin) >> 12;
 			int vx = (- u * Sin + v * Cos) >> 12;			

      u = ((ux * Sat) >> 9) + 128;
 			v = ((vx * Sat) >> 9) + 128;
	  	
      ptr[1] = std::min(std::max(0, u), 255);
	  	ptr[3] = std::min(std::max(0, v), 255);
    }		
  }

  return frame;
}


} } } //namespace avs::filters::tweak
