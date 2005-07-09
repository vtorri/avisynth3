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
#include "interleaved.h"
#include "../../colorspace.h"
#include "../../exception/nosuchplane.h"

//assert include
#include <assert.h>


namespace avs { namespace videoframe { namespace concrete {



Interleaved::Interleaved(PColorSpace const& space, Dimension const& dim, FrameType type, PEnvironment const& env)
  : Base( space, dim, type )
  , main_( space->ToPlaneDim(dim, NOT_PLANAR), env ) { }


Interleaved::Interleaved(PColorSpace const& space, Dimension const& dim, FrameType type, BufferWindow const& main)
  : Base( space, dim, type )
  , main_( main )
{
  assert( main.GetDimension() == space->ToPlaneDim(dim, NOT_PLANAR) );   //check the given buffer fits
}



BufferWindow& Interleaved::operator[](char plane)
{
  if ( plane != '\0' )
    throw exception::NoSuchPlane(GetColorSpace(), plane);
  return GetMain();
}


BufferWindow const& Interleaved::operator[](char plane) const
{
  if ( plane != '\0' )
    throw exception::NoSuchPlane(GetColorSpace(), plane);
  return GetConstMain();
}



} } } //namespace avs::vframe::concrete

