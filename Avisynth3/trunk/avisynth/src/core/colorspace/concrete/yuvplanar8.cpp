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
#include "yuvplanar8.h"
#include "../../videoframe.h"
#include "../../../utility/bytemap.h"
#include "../../../utility/bytecounter.h"


namespace avs { namespace cspace { namespace concrete {



bool YuvPlanar8::HasProperty(Property prop) const
{
  return prop == P_PLANAR || prop == P_YUV || prop == P_DEPTH8;
}



void YuvPlanar8::Count(VideoFrame const& frame, ByteCounter& y, ByteCounter& u, ByteCounter& v) const
{
  y.Count<1>(frame.ReadFrom(PLANAR_Y));            //counts Y values
  u.Count<1>(frame.ReadFrom(PLANAR_U));            //counts U values
  v.Count<1>(frame.ReadFrom(PLANAR_V));            //counts V values
 
}


void YuvPlanar8::Apply(VideoFrame& frame, ByteMap const& y, ByteMap const& u, ByteMap const& v) const
{
  y.ApplyTo<1>(frame.ReadFrom(PLANAR_Y));          //looks up Y values 
  u.ApplyTo<1>(frame.ReadFrom(PLANAR_U));          //looks up U values
  v.ApplyTo<1>(frame.ReadFrom(PLANAR_V));          //looks up V values
}



} } } //namespace avs::cspace::concrete
