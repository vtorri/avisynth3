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


#include "colorspace.h"


void ColorSpace::WidthSignCheck(int width) const
{
  static const string ERR_MSG = "Width cannot be negative or null";
  if (width <= 0)
    throw ColorSpaceConstraintViolation(*this, ERR_MSG);
}

void ColorSpace::HeightSignCheck(int height) const
{
  static const string ERR_MSG = "Height cannot be negative or null";
  if (height <= 0)
    throw ColorSpaceConstraintViolation(*this, ERR_MSG);
}

const string& ColorSpace::GetName() const
{
  static const string names[] = { "RGB24", "RGB32", "YUY2", "YV12" };
  return names[id];
}



void InterLeaved::IsLegalHeightShift(int shift, bool interlaced) const
{
  static const string ERR_MSG = "Interlaced Height must be mod 2";
  if (interlaced && (shift & 1))
    throw ColorSpaceConstraintViolation(*this, ERR_MSG);
}

int InterLeaved::WidthToTowSize(int width, Plane plane) const throw(NoSuchPlaneException)
{
  if (plane != NOT_PLANAR)
    throw NoSuchPlaneException();
  return BytesPerPixel*width;
}

int InterLeaved::HeightToPlaneHeight(int height, Plane plane) const throw(NoSuchPlaneException)
{
  if (plane != NOT_PLANAR)
    throw NoSuchPlaneException();
  return height;
}


void ColorSpace::YUVIsLegalWidthShift(int shift) const throw(ColorSpaceConstraintViolation)
{
  static const string ERR_MSG = "Width must be mod 2";
  if (shift & 1)
    throw ColorSpaceConstraintViolation(*this, ERR_MSG);
}


void YUY2::IsLegalHeightShift(int shift, bool interlaced) const
{
  static const string ERR_MSG = "Interlaced Height must be mod 2";
  if (interlaced && (shift & 1))
    throw ColorSpaceConstraintViolation(*this, ERR_MSG);
}


void YV12::IsLegalHeightShift(int shift, bool interlaced) const
{
  static const string ERR_MSG = "Height must be mod 2";
  static const string ERR_MSG_INT = "Interlaced Height must be mod 4";
  if (interlaced && (shift & 3))
    throw ColorSpaceConstraintViolation(*this, ERR_MSG_INT);
  if (shift & 1)
    throw ColorSpaceConstraintViolation(*this, ERR_MSG);
}
