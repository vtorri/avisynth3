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


#ifndef __AVS_COLORSPACE_H__
#define __AVS_COLORSPACE_H__

//avisynth includes
#include "forward.h"
#include "frametype.h"
#include "../define.h"             //for AVS_NOVTABLE
#include "cow_shared_ptr.h"        //so PVideoFrame is defined
#include "geometry/vecteur.h"
#include "geometry/dimension.h"

//boost includes
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>


namespace avs {



//////////////////////////////////////////////////////////////////////////////
//  ColorSpace
//
//  polymorphic class representing a video color space
//
class AVS_NOVTABLE ColorSpace : public boost::enable_shared_from_this<ColorSpace const>
{

public:

  //ColorSpace ids, used to switch on color spaces
  enum ID
  {
    I_EXTERNAL,       //reports an external colorspace (defined by a plugin)
    I_RGB24,
    I_RGB32,
    I_YUY2,
    I_YV12,
    I_YV24,
  };


public:  //structors

  ColorSpace() { }
  virtual ~ColorSpace() { }


public:  //ColorSpace interface

  virtual ID id() const = 0;
  virtual char const * GetName() const = 0;

  virtual bool HasProperty(char const * prop) const = 0;
  virtual bool HasPlane(char plane) const = 0;
  
  //  virtual unsigned long GetFourCC() const = 0;
//  virtual long GetBitsPerPixel() const = 0;
  
//  virtual long GetBitmapSize(Dimension const& dim) const = 0;  //NB: bitmaps are 4 bytes aligned


  virtual void Check(long x, long y, bool interlaced = false) const = 0;

  //convenience versions of Check
  void CheckDim(Dimension const& dim, bool interlaced = false) const { Check(dim.GetWidth(), dim.GetHeight(), interlaced); }
  void CheckVect(Vecteur const& vect, bool interlaced = false) const { Check(vect.x, vect.y, interlaced); }


  //method to convert frame coords into to plane coords
  //unlike the above, it does not check validity, but just perform the operation   
  virtual void ToPlane(long& x, long& y, char plane) const = 0;
 
  //convenience versions of ToPlane
  Dimension ToPlaneDim(Dimension const& dim, char plane) const
  {
    long x = dim.GetWidth(), y = dim.GetHeight();
    ToPlane(x, y, plane);
    return Dimension(x, y);
  }
  
  Vecteur ToPlaneVect(Vecteur const& vect, char plane) const
  {
    Vecteur result(vect);
    ToPlane(result.x, result.y, plane);
    return result;
  }


public:  //blank frame creation method

  virtual PVideoFrame CreateFrame(PEnvironment const& env, Dimension const& dim, FrameType type) const = 0;


public:  //comparison operators

  virtual bool operator== (ColorSpace const& other) const { return this == &other; }
  virtual bool operator!= (ColorSpace const& other) const { return this != &other; }


public:  //helper queries

  bool IsRGB24() const { return id() == I_RGB24; }
  bool IsRGB32() const { return id() == I_RGB32; }

  bool IsYUY2() const { return id() == I_YUY2; }
  bool IsYV12() const { return id() == I_YV12; }
  bool IsYV24() const { return id() == I_YV24; }

  bool IsRGB() const { return HasProperty("RGB"); }
  bool IsYUV() const { return HasProperty("YUV"); }
  bool IsPlanar() const { return HasProperty("PLANAR"); }
  bool IsInterleaved() const { return HasProperty("INTERLEAVED"); }

};



} //namespace avs

#endif //__AVS_COLORSPACE_H__
