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


#ifndef __AVS_CS_PLANAR_YUV_H__
#define __AVS_CS_PLANAR_YUV_H__

//avisynh includes
#include "../colorspace.h"
#include "../videoframe/planar_yuv.h"
#include "../exception/nosuchplane.h"


namespace avs { namespace cspace {




/////////////////////////////////////////////////////////////////////////////////
//  YV12
//
//  YV12 ColorSpace subclass
//
class YV12 : public ColorSpace
{

private:  //constructor

  YV12() { }


public:  //ColorSpace interface

  virtual ID id() const { return I_YV12; }
  virtual char const * GetName() const { return "YV12"; }

  virtual bool HasProperty(Property prop) const
  {
    return prop == PLANAR || prop == YUV || prop == DEPTH8;
  }
  virtual bool HasPlane(Plane plane) const
  {
    return plane == PLANAR_Y || plane == PLANAR_U || plane == PLANAR_V;
  }


  virtual void Check(int x, int y, bool interlaced = false) const
  {
    if ( x & 1 )                          //x must be mod 2
      ThrowInvalidWidthException(2, x);
    if ( interlaced && (y & 3) )          //y must be mod 4 when interlaced             
      ThrowInvalidInterlacedHeightException(4, y);
    if ( y & 1 )                          //y must be mod 2 when not interlaced
      ThrowInvalidHeightException(2, y);
  }

  virtual void ToPlane(int& x, int& y, Plane plane) const
  {
    switch(plane)
    {
    case PLANAR_Y: break;
    case PLANAR_U:
    case PLANAR_V: x >>= 1; y >>= 1; break;

    default:
      throw exception::NoSuchPlane(*this, plane);
    }
  }

  virtual PVideoFrame CreateFrame(PEnvironment const& env, Dimension const& dim, FrameType type) const
  {
    return CPVideoFrame( static_cast<VideoFrame *>(new vframe::YV12(dim, type, env)) );
  }


public:  //instance

  static YV12 instance;

};




/////////////////////////////////////////////////////////////////////////////////
//  YV24
//
//  YV24 ColorSpace subclass
//
class YV24 : public ColorSpace
{

private:  //constructor

  YV24() { }


public:  //ColorSpace interface

  virtual ID id() const { return I_YV24; }
  virtual char const * GetName() const { return "YV24"; }

  virtual bool HasProperty(Property prop) const
  {
    return prop == PLANAR || prop == YUV || prop == DEPTH8;
  }
  virtual bool HasPlane(Plane plane) const
  {
    return plane == PLANAR_Y || plane == PLANAR_U || plane == PLANAR_V;
  }


  virtual void ToPlane(int& /*x*/, int& /*y*/, Plane plane) const
  {
    if ( ! HasPlane(plane) )
      throw exception::NoSuchPlane(*this, plane);
  }

  virtual PVideoFrame CreateFrame(PEnvironment const& env, Dimension const& dim, FrameType type) const
  {
    return CPVideoFrame( static_cast<VideoFrame *>(new vframe::YV24(dim, type, env)) );
  }


public:  //instance

  static YV24 instance;

};



/////////////////////////////////////////////////////////////////////////////////
//  YV45
//
//  YV45 ColorSpace subclass
//
class YV45 : public ColorSpace
{

private:  //constructor

  YV45() { }


public:  //ColorSpace interface

  virtual ID id() const { return I_YV45; }
  virtual char const * GetName() const { return "YV45"; }

  virtual bool HasProperty(Property prop) const
  {
    return prop == PLANAR || prop == YUV || prop == DEPTH15;
  }
  virtual bool HasPlane(Plane plane) const
  {
    return plane == PLANAR_Y || plane == PLANAR_U || plane == PLANAR_V;
  }


  virtual void ToPlane(int& x, int& /*y*/, Plane plane) const
  {
    if ( ! HasPlane(plane) )
      throw exception::NoSuchPlane(*this, plane);

    x <<= 1;
  }

  virtual PVideoFrame CreateFrame(PEnvironment const& env, Dimension const& dim, FrameType type) const
  {
    return CPVideoFrame( static_cast<VideoFrame *>(new vframe::YV45(dim, type, env)) );
  }


public:  //instance

  static YV45 instance;

};





} } //namespace avs::cspace

#endif //__AVS_CS_PLANAR_YUV_H__
