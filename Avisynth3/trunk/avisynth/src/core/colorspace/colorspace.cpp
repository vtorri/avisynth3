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
#include "rgb.h"
#include "yuy2.h"
#include "planar_yuv.h"
#include "../dimension.h"
#include "../exception.h"
#include "../exception/unsupportedcolorspace.h"

//boost include
#include <boost/format.hpp>


namespace avs {


void ColorSpace::CheckDim(Dimension const& dim, bool interlaced) const
{
  Check(dim.GetWidth(), dim.GetHeight(), interlaced);
}

void ColorSpace::CheckVect(Vecteur const& vect, bool interlaced) const
{
  Check(vect.x, vect.y, interlaced);
}

Dimension ColorSpace::ToPlaneDim(Dimension const& dim, Plane plane) const
{
  int x = dim.GetWidth(), y = dim.GetHeight();
  ToPlane(x, y, plane);
  return Dimension(x, y);
}

Vecteur ColorSpace::ToPlaneVect(Vecteur const& vect, Plane plane) const
{
  Vecteur result = vect;
  ToPlane(result.x, result.y, plane);
  return result;
}


ColorSpace& ColorSpace::rgb24() { return cspace::RGB24::instance; }
ColorSpace& ColorSpace::rgb32() { return cspace::RGB32::instance; }
ColorSpace& ColorSpace::rgb45() { return cspace::RGB45::instance; }
ColorSpace& ColorSpace::yuy2() { return cspace::YUY2::instance; }
ColorSpace& ColorSpace::yv12() { return cspace::YV12::instance; }
ColorSpace& ColorSpace::yv24() { return cspace::YV24::instance; }
ColorSpace& ColorSpace::yv45() { return cspace::YV45::instance; }



void ColorSpace::ThrowUnsupportedColorSpaceException() const
{
  throw exception::UnsupportedColorSpace(*this);
}

void ColorSpace::ThrowNoSuchPlaneException(Plane plane) const
{
  throw GenericException(str( boost::format("%s doesn't have plane %s") % GetName() % NameOf(plane) ));
}


void ColorSpace::ThrowInvalidInterlacedHeightException(int modulo, int height) const
{
  throw GenericException(str( boost::format("%s: interlaced height must be mod %d") % GetName() % modulo ));
}

void ColorSpace::ThrowInvalidHeightException(int modulo, int height) const
{
  throw GenericException(str( boost::format("%s: height must be mod %d") % GetName() % modulo ));
}

void ColorSpace::ThrowInvalidWidthException(int modulo, int width) const
{
  throw GenericException(str( boost::format("%s: width must be mod %d") % GetName() % modulo ));
}




namespace cspace {


RGB24 RGB24::instance;
RGB32 RGB32::instance;
RGB45 RGB45::instance;
YUY2 YUY2::instance;
YV12 YV12::instance;
YV24 YV24::instance;
YV45 YV45::instance;


} } //namespace avs::cspace

