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
#include "yuy2.h"
#include "../../../utility/bytemap.h"
#include "../../../utility/bytecounter.h"
#include "../../exception/colorspace/invalidwidth.h"


namespace avs { namespace cspace { namespace concrete {



bool YUY2::HasProperty(Property prop) const
{ 
  return prop == P_YUV || prop == P_INTERLEAVED || prop == P_DEPTH8;
}


void YUY2::Check(int x, int y, bool interlaced) const 
{
  InterleavedType::Check(x, y, interlaced);
  if ( x & 1 )                                                        //if x is not even
    throw exception::cspace::InvalidWidth(shared_from_this(), x, 2);  //exception
}



void YUY2::Count(VideoFrame const& frame, ByteCounter& y, ByteCounter& u, ByteCounter& v) const
{
  CWindowPtr wp = frame.ReadFrom(NOT_PLANAR);

  y.Count<2>(wp);            //counts Y values

  wp.to(1, 0);
  y.Count<4>(wp);            //counts U values

  wp.to(2, 0);
  v.Count<4>(wp);            //counts V values
}


void YUY2::Apply(VideoFrame& frame, ByteMap const& y, ByteMap const& u, ByteMap const& v) const
{
  WindowPtr wp = frame.WriteTo(NOT_PLANAR);

  y.ApplyTo<2>(wp);          //looks up Y values 

  wp.to(1, 0);
  u.ApplyTo<4>(wp);          //looks up U values

  wp.to(2, 0);
  v.ApplyTo<4>(wp);          //looks up V values
}



} } } //namespace avs::cspace::concrete
