// Avisynth v3.0 alpha.  Copyright 2004 David Pierre - Ben Rudiak-Gould et al.
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


//avisynth include
#include "planar_yuv.h"
#include "../../colorspace.h"
#include "../../exception/nosuchplane.h"


//assert include
#include <assert.h>


namespace avs { namespace vframe { namespace concrete {



PlanarYUV::PlanarYUV(PColorSpace const& space, Dimension const& dim, FrameType type, PEnvironment const& env)
  : Base( space, dim, type )
  , y_( space->ToPlaneDim(dim, PLANAR_Y), env )
  , u_( space->ToPlaneDim(dim, PLANAR_U), env )
  , v_( space->ToPlaneDim(dim, PLANAR_V), env ) { }


PlanarYUV::PlanarYUV(PColorSpace const& space, Dimension const& dim, FrameType type, BufferWindow const& y, BufferWindow const& u, BufferWindow const& v)
  : Base( space, dim, type )
  , y_( y )
  , u_( u )
  , v_( v )
{
  assert( y.GetDimension() == space->ToPlaneDim(dim, PLANAR_Y) );
  assert( u.GetDimension() == space->ToPlaneDim(dim, PLANAR_U) );
  assert( v.GetDimension() == space->ToPlaneDim(dim, PLANAR_V) );
  assert( y.GetEnvironment() == u.GetEnvironment() && y.GetEnvironment() == v.GetEnvironment() );
}



PlanarYUV::BufferWindow& operator[](Plane plane)  
{
  switch( plane )
  {
  case PLANAR_Y: return GetY();
  case PLANAR_U: return GetU();
  case PLANAR_V: return GetV();
    
  default: throw exception::NoSuchPlane(GetColorSpace(), plane);  
  }   
}

  virtual BufferWindow const& operator[](Plane plane) const
  {
    switch(plane)
    {
    case PLANAR_Y: return GetConstY();
    case PLANAR_U: return GetConstU();
    case PLANAR_V: return GetConstV();

    default: throw exception::NoSuchPlane(GetColorSpace(), plane);
    }
  }




} } } //namespace avs::vframe::concrete
