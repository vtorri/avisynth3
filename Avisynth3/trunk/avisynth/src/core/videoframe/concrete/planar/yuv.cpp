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
#include "./yuv.h"
#include "../../../colorspace.h"
#include "../../../exception/nosuchplane.h"

//assert include
#include <assert.h>


namespace avs { namespace videoframe { namespace concrete { namespace planar {



YUV::YUV(PColorSpace const& space, Dimension const& dim, FrameType type, PEnvironment const& env)
  : Base( space, dim, type )
  , y_( space->ToPlaneDim(dim, 'Y'), env )
  , u_( space->ToPlaneDim(dim, 'U'), env )
  , v_( space->ToPlaneDim(dim, 'V'), env ) { }


YUV::YUV(PColorSpace const& space, Dimension const& dim, FrameType type, BufferWindow const& y, BufferWindow const& u, BufferWindow const& v)
  : Base( space, dim, type )
  , y_( y )
  , u_( u )
  , v_( v )
{
  assert( y.GetDimension() == space->ToPlaneDim(dim, 'Y') );
  assert( u.GetDimension() == space->ToPlaneDim(dim, 'U') );
  assert( v.GetDimension() == space->ToPlaneDim(dim, 'V') );
  assert( y.GetEnvironment() == u.GetEnvironment() && y.GetEnvironment() == v.GetEnvironment() );
}



BufferWindow& YUV::operator[](char plane)  
{
  switch( plane )
  {
  case 'Y': return GetY();
  case 'U': return GetU();
  case 'V': return GetV();
    
  default: throw exception::NoSuchPlane(GetColorSpace(), plane);  
  }   
}


BufferWindow const& YUV::operator[](char plane) const
{
  switch( plane )
  {
  case 'Y': return GetConstY();
  case 'U': return GetConstU();
  case 'V': return GetConstV();

  default: throw exception::NoSuchPlane(GetColorSpace(), plane);  
  }  
}



} } } } //namespace avs::videoframe::concrete::planar
