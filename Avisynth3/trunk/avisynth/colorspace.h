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

#include "geometric.h"         //which includes string
#pragma warning(disable:4290)  //to get rid of the C++ exception Notification ignored
using namespace std;

//ColorSpace ids
enum CS_ID {
  I_RGB24,
  I_RGB32,
  I_YUY2,
  I_YV12
};

//planes
enum Plane {
  NOT_PLANAR,
  PLANAR_Y,
  PLANAR_U,
  PLANAR_V
};

#define PLANE_COUNT 4   //number of planes, used to loop over planes with HasPlane


//exception class for requesting about a plane who don't exist
//subclass of invalid_argument
class invalid_plane : public invalid_argument {

public:
  invalid_plane() : invalid_argument("No such plane") { }
}; 



class ColorSpace {

protected:
  //factorisation of a common check of YV12 and YUY2
  void YUVIsLegalWidthShift(int shift) const throw(invalid_argument)
  {
    if (shift & 1)
      ThrowError(": Width must be mod 2");
  }

  void ThrowError(const string& err_msg) const { throw invalid_argument(GetName() + err_msg); }

public:
  ColorSpace(CS_ID _id) : id(_id) { } 

  const CS_ID id;   //ColorSpace id, used to switch on ColorSpaces

  bool operator== (const ColorSpace& other) const { return id == other.id; }
  bool operator!= (const ColorSpace& other) const { return id != other.id; }

  //return the colorspace name ("RGB24" and co..)
  const string& GetName() const
  {
    static const string names[] = { "RGB24", "RGB32", "YUY2", "YV12" };
    return names[id];
  }

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

  virtual void IsLegalCoords(const Vecteur& coords, bool interlaced = false) const throw(invalid_argument) = 0;

  //methods to convert frame coords into to plane coords
  //unlike the above, they does not check validity, but just perform the operation   
  virtual Vecteur ToPlaneCoords(const Vecteur& vect, Plane plane) const throw(invalid_plane) = 0;

};


class InterLeaved : public ColorSpace {

  int BytesPerPixel;

public:
  InterLeaved(CS_ID _id, int bpp) : ColorSpace(_id), BytesPerPixel(bpp) { }

  //HasProperty for RGB24 & 32 cases
  virtual bool HasProperty(Property prop) const { return prop == ColorSpace::RGB || prop == ColorSpace::INTERLEAVED; }


  virtual void IsLegalCoords(const Vecteur& coords, bool interlaced = false) const throw(invalid_argument)
  {
    if (interlaced && (coords.GetY() & 1))
      ThrowError(": Interlaced Height must be mod 2");
  }

  virtual Vecteur ToPlaneCoords(const Vecteur& coords, Plane plane) const throw(invalid_plane)
  {
    if (plane != NOT_PLANAR)
      throw invalid_plane();
    return Vecteur(coords.GetX() * BytesPerPixel, coords.GetY());
  }

  int GetBytesPerPixel() const { return BytesPerPixel; }
};


class RGB24 : public InterLeaved {

  RGB24() : InterLeaved(I_RGB24, 3) { }

public:
  static const RGB24 instance;
};


class RGB32 : public InterLeaved {

  RGB32() : InterLeaved(I_RGB32, 4) { }

public:
  static const RGB32 instance;
};




class YUY2 : public InterLeaved {

  YUY2() : InterLeaved(I_YUY2, 1) { }

public:
  static const YUY2 instance;

  virtual bool HasProperty(Property prop) const { return prop == ColorSpace::YUV || prop == ColorSpace::INTERLEAVED; }

  virtual void IsLegalCoords(const Vecteur& coords, bool interlaced = false) const throw(invalid_argument)
  {
    YUVIsLegalWidthShift(coords.GetX());
    InterLeaved::IsLegalCoords(coords, interlaced);
  }


};



class YV12 : public ColorSpace {

  YV12() : ColorSpace(I_YV12) { }

public:
  static const YV12 instance;

  virtual bool HasProperty(Property prop) const { return prop == ColorSpace::YUV || prop == ColorSpace::PLANAR; }
  virtual bool HasPlane(Plane plane) const { return plane != NOT_PLANAR; }
  
  virtual void IsLegalCoords(const Vecteur& coords, bool interlaced = false) const throw(invalid_argument)
  {
    YUVIsLegalWidthShift(coords.GetX());
    if (interlaced && (coords.GetY() & 3))
      ThrowError(": Height must be mod 2");
    if (coords.GetY() & 1)
      ThrowError(": Interlaced Height must be mod 4");    
  }

  virtual Vecteur ToPlaneCoords(const Vecteur& coords, Plane plane) const throw(invalid_plane)
  {
    switch(plane)
    {
      case PLANAR_Y: return coords;
      case PLANAR_U:
      case PLANAR_V: return Vecteur(coords.GetX()/2, coords.GetY()/2);
    }
    throw invalid_plane();
  }
};

#endif //__COLORSPACE_H__