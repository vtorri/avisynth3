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
#include "interleaved.h"
#include "../colorspace.h"
#include "../bufferwindow/copier.h"
#include "../exception/nosuchplane.h"
#include "../bufferwindow/sizechanger.h"


namespace avs { namespace vframe {



InterleavedBase::InterleavedBase(ColorSpace& space, Dimension const& dim, FrameType type, PEnvironment const& env)
  : Base( space, dim, type )
  , main_( space.ToPlaneDim(dim, NOT_PLANAR), env ) { }

InterleavedBase::InterleavedBase(ColorSpace& space, Base const& other)
  : Base( space, other )
  , main_( space.ToPlaneDim(other.GetDimension(), NOT_PLANAR), other.GetEnvironment() ) { }



BufferWindow& InterleavedBase::operator[](Plane plane)
{
  if ( plane != NOT_PLANAR )
    throw exception::NoSuchPlane(GetColorSpace(), plane);
  return GetMain();
}

BufferWindow const& InterleavedBase::operator[](Plane plane) const
{
  if ( plane != NOT_PLANAR )
    throw exception::NoSuchPlane(GetColorSpace(), plane);
  return GetConstMain();
}


void InterleavedBase::ChangeSize(Vecteur const& topLeft, Vecteur const& bottomRight)
{
  ColorSpace & space = GetColorSpace();

  //verify params validity
  space.CheckVect(topLeft, MaybeInterlaced());
  space.CheckVect(bottomRight, MaybeInterlaced());

  //make changes
  bw::SizeChanger()(GetMain(), space.ToPlaneVect(topLeft, NOT_PLANAR), space.ToPlaneVect(bottomRight, NOT_PLANAR) );
  SetDimension( Dimension(GetDimension()) += bottomRight - topLeft );
  //NB: space.Check test skipped, we know dim is ok since bottomRight and topLeft are
}

void InterleavedBase::Copy(VideoFrame const& other, Vecteur const& coords)
{
  ColorSpace& space = GetColorSpace();

  if ( other.GetColorSpace() == space )
  {
    space.CheckVect(coords, MaybeInterlaced());
    bw::Copier()( other[NOT_PLANAR], GetMain(), space.ToPlaneVect(coords, NOT_PLANAR) );
  }
}


void InterleavedBase::TurnLeft()
{
  Dimension dim = GetDimension().Turn<1>();
  GetColorSpace().CheckDim(dim);
  GetMain() = DoTurnLeft();
  SetDimension(dim);
}

void InterleavedBase::TurnRight()
{
  Dimension dim = GetDimension().Turn<1>();
  GetColorSpace().CheckDim(dim);
  GetMain() = DoTurnRight();
  SetDimension(dim);
}

void InterleavedBase::Blend(VideoFrame const& other, float factor)
{
  if ( GetColorSpace() == other.GetColorSpace() )
    DoBlend( other[NOT_PLANAR], factor );
}



} } //namespace avs::vframe
