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


#ifndef __AVS_COLORSPACE_H__
#define __AVS_COLORSPACE_H__

//avisynth includes
#include "plane.h"
#include "vecteur.h"
#include "dimension.h"
#include "frametype.h"
#include "smart_ptr_fwd.h"

//boost include
#include <boost/utility.hpp>    //for noncopyable


namespace avs {



//////////////////////////////////////////////////////////////////////////////
//  ColorSpace
//
//  polymorphic class representing a video color space
//
class ColorSpace : private boost::noncopyable
{

public:

  //ColorSpace ids, used to switch on color spaces
  enum ID
  {
    I_RGB24,
    I_RGB32,
    I_RGB45,
    I_YUY2,
    I_YV12,
    I_YV24,
    I_YV45
  };

  //ColorSpace properties
  enum Property
  {
    RGB,
    YUV,
    INTERLEAVED,
    PLANAR,
    DEPTH8,
    DEPTH15
  };


public:  //constructor

  ColorSpace() { }


public:  //ColorSpace interface

  virtual ID id() const = 0;
  virtual char const * GetName() const = 0;

  virtual bool HasProperty(Property prop) const = 0;
  virtual bool HasPlane(Plane plane) const = 0;

  virtual void Check(int x, int y, bool interlaced = false) const
  {
    if ( interlaced && (y & 1) )
      ThrowInvalidInterlacedHeightException(2, y);
  }

  virtual void CheckDim(Dimension const& dim, bool interlaced = false) const;
  virtual void CheckVect(Vecteur const& vect, bool interlaced = false) const;


  //method to convert frame coords into to plane coords
  //unlike the above, it does not check validity, but just perform the operation   
  virtual void ToPlane(int& x, int& y, Plane plane) const = 0;
  
  virtual Dimension ToPlaneDim(Dimension const& dim, Plane plane) const;
  virtual Vecteur ToPlaneVect(Vecteur const& vect, Plane plane) const;

  //method to create a frame of the given colorspace
  virtual CPVideoFrame CreateFrame(PEnvironment const& env, Dimension const& dim, FrameType type) const = 0;

  bool operator== (const ColorSpace& other) const { return this == &other; }
  bool operator!= (const ColorSpace& other) const { return this != &other; }


public:  //helper queries

  bool IsRGB24() const { return id() == I_RGB24; }
  bool IsRGB32() const { return id() == I_RGB32; }
  bool IsRGB45() const { return id() == I_RGB45; }

  bool IsYUY2() const { return id() == I_YUY2; }
  bool IsYV12() const { return id() == I_YV12; }
  bool IsYV24() const { return id() == I_YV24; }
  bool IsYV45() const { return id() == I_YV45; }

  bool IsRGB() const { return HasProperty(RGB); }
  bool IsYUV() const { return HasProperty(YUV); }
  bool IsInterleaved() const { return HasProperty(INTERLEAVED); }
  bool IsPlanar() const { return HasProperty(PLANAR); }
  bool IsDepth8() const { return HasProperty(DEPTH8); }
  bool IsDepth15() const { return HasProperty(DEPTH15); }


public:  //access to relevant instances

  static ColorSpace & rgb24();
  static ColorSpace & rgb32();
  static ColorSpace & rgb45();
  static ColorSpace & yuy2();
  static ColorSpace & yv12();
  static ColorSpace & yv24();
  static ColorSpace & yv45();


public:  //exception helper methods

  __declspec(noreturn) void ThrowUnsupportedColorSpaceException() const;
  __declspec(noreturn) void ThrowNoSuchPlaneException(Plane plane) const;

  __declspec(noreturn) void ThrowInvalidInterlacedHeightException(int modulo, int height) const;
  __declspec(noreturn) void ThrowInvalidHeightException(int modulo, int height) const;
  __declspec(noreturn) void ThrowInvalidWidthException(int modulo, int width) const;

};//ColorSpace




} //namespace avs

#endif //__AVS_COLORSPACE_H__