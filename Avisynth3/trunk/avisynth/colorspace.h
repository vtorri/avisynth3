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


#ifndef __COLORSPACE_H__
#define __COLORSPACE_H__

#include <string>
#include "property.h"
using namespace std;

//ColorSpace ids
enum CS_ID {
  I_RGB24,
  I_RGB32,
  I_YUY2,
  I_YV12
  //I_I420
};

enum Plane {
  NOT_PLANAR,
  PLANAR_Y,
  PLANAR_U,
  PLANAR_V
};

class NoSuchPlaneException { };

class ColorSpace;

class ColorSpaceConstraintViolation : public ConstraintViolation {

public:
  ColorSpaceConstraintViolation(const ColorSpace& space, const string& msg);

};


class ColorSpace {

protected:
  void WidthSignCheck(int width) const throw(ColorSpaceConstraintViolation);
  void HeightSignCheck(int height) const throw(ColorSpaceConstraintViolation);

  void YUVIsLegalWidthShift(int shift) const throw(ColorSpaceConstraintViolation);

public:
  ColorSpace(CS_ID _id) : id(_id) { } 

  const CS_ID id;   //ColorSpace id, used to switch on ColorSpaces

  bool operator== (const ColorSpace& other) const { return id == other.id; }
  bool operator!= (const ColorSpace& other) const { return id != other.id; }

  //return the colorspace name ("RGB24" and co..)
  const string& GetName() const; 

  //ColorSpace properties
  enum Property {
    RGB,
    YUV,
    INTERLEAVED,
    PLANAR,
  };

  virtual bool HasProperty(Property prop) const = 0;
  virtual bool HasPlane(Plane plane) const { return plane == NOT_PLANAR; } //default implementation not planar case

  //helper functions of the above
  bool IsRGB() const { return HasProperty(RGB); }
  bool IsYUV() const { return HasProperty(YUV); }

  bool IsInterleaved() const { return HasProperty(INTERLEAVED); }
  bool IsPlanar() const { return HasProperty(PLANAR); }

  //methods to check width and height constraints
  virtual void IsLegalWidthShift(int shift) const throw(ColorSpaceConstraintViolation) = 0;
  void IsLegalWidth(int width) const throw(ColorSpaceConstraintViolation) { WidthSignCheck(width); IsLegalWidthShift(width); }

  virtual void IsLegalHeightShift(int shift, bool interlaced = false) const throw(ColorSpaceConstraintViolation) = 0;
  void IsLegalHeight(int height, bool interlaced = false) const throw(ColorSpaceConstraintViolation) { HeightSignCheck(height); IsLegalHeightShift(height, interlaced); }

  //methods to convert width and height to plane sizes
  //unlike the above, they just to the operation 
  virtual int WidthToRowSize(int width, Plane plane) const throw(NoSuchPlaneException) = 0;
  virtual int HeightToPlaneHeight(int height, Plane plane) const throw(NoSuchPlaneException) = 0;

};

class InterLeaved : public ColorSpace {

  int BytesPerPixel;

public:
  InterLeaved(CS_ID _id, int bpp) : ColorSpace(_id), BytesPerPixel(bpp) { }

  virtual void IsLegalHeightShift(int shift, bool interlaced = false) const throw(ColorSpaceConstraintViolation) = 0;

  virtual int WidthToRowSize(int width, Plane plane) const throw(NoSuchPlaneException);
  virtual int HeightToPlaneHeight(int height, Plane plane) const throw(NoSuchPlaneException);  
};

//common ancestor of RGB24 & 32
//since RGB seems to be a macro, used the name CS_RGB
class CS_RGB : public InterLeaved {

public:
  CS_RGB(CS_ID _id, int bpp) : InterLeaved(_id, bpp) { }

  virtual bool HasProperty(Property prop) const { return prop == ColorSpace::RGB || prop == ColorSpace::INTERLEAVED; }

  virtual void IsLegalWidthShift(int shift) const throw(ColorSpaceConstraintViolation) { }
};


class RGB24 : public CS_RGB {

  RGB24() : CS_RGB(I_RGB24, 3) { }

public:
  static const RGB24 instance;

};


class RGB32 : public CS_RGB {

  RGB32() : CS_RGB(I_RGB32, 4) { }

public:
  static const RGB32 instance;

};



class YUY2 : public InterLeaved {

  YUY2() : InterLeaved(I_YUY2, 1) { }

public:
  static const YUY2 instance;

  virtual bool HasProperty(Property prop) const { return prop == ColorSpace::YUV || prop == ColorSpace::INTERLEAVED; }

  virtual void IsLegalHeightShift(int shift, bool interlaced = false) const throw(ColorSpaceConstraintViolation);  
  virtual void IsLegalWidthShift(int shift) const throw(ColorSpaceConstraintViolation) { YUVIsLegalWidthShift(shift); }

};



class YV12 : public ColorSpace {

  YV12() : ColorSpace(I_YV12) { }

public:
  static const YV12 instance;

  virtual bool HasProperty(Property prop) const { return prop == ColorSpace::YUV || prop == ColorSpace::PLANAR; }
  virtual bool HasPlane(Plane plane) const { return plane != NOT_PLANAR; }
  
  virtual void IsLegalHeightShift(int shift, bool interlaced = false) const;
  virtual void IsLegalWidthShift(int shift) const throw(ColorSpaceConstraintViolation) { YUVIsLegalWidthShift(shift); }

};

#endif //__COLORSPACE_H__