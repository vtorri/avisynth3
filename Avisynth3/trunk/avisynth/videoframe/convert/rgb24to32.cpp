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
#include "../rgb24videoframe.h"
#include "../rgb32videoframe.h"
#include "../../colorspace/rgb32.h"


namespace avs {


RGB32VideoFrame::RGB32VideoFrame(const RGB24VideoFrame& other)
  : InterleavedVideoFrame( CS::RGB32::instance(), other )
{

  CWindowPtr src = other.GetMain().GetReadPtr();
  WindowPtr dst = GetMain().GetWritePtr();

  const Dimension dim = GetDimension();

  for ( int y = dim.GetHeight(); y-- > 0; src.to(0, 1), dst.to(0, 1) ) 
  {
    const BYTE * p = src.ptr;
    BYTE * q = dst.ptr;

    for ( int x = dim.GetWidth(); x-- > 0; )
    {
      *(q++) = *(p++);     //copy 1st 3 bytes
      *(q++) = *(p++);
      *(q++) = *(p++);
      *(q++) = 255;        //4th is 255, and p/q in place for next pixel
    }
  }

}



}; //namespace avs