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


#ifndef __VIDEOFRAME_H__
#define __VIDEOFRAME_H__


#include "pclip.h"
#include "bufferwindow.h"
#include "colorspace.h"
#include "property.h" 
#include "videoinfo.h"


#include <string> 
using namespace std;





class TaggedVideoFrame : public VideoFrame {

protected:
  CPPropertySet propSet;

  //plane fetch methods (must be consistent together)
  //their existence allow to define many things at this level
  virtual const BufferWindow& GetPlane(Plane plane) const throw(NoSuchPlaneException) = 0;
  virtual BufferWindow& GetPlane(Plane pane) throw(NoSuchPlaneException) = 0;
  
public:
  //copy constructor
  TaggedVideoFrame(const TaggedVideoFrame& other) : propSet(other.propSet) { }
  //conversion construtor
  TaggedVideoFrame(const ColorSpace& space, const TaggedVideoFrame& other);
  //normal constructor
  TaggedVideoFrame(const ColorSpace& space, int width, int height, bool IsField);


  virtual int GetPitch(Plane plane) const throw(NoSuchPlaneException) { return GetPlane(plane).GetPitch(); }
  virtual int GetRowSize(Plane plane) const throw(NoSuchPlaneException) { return GetPlane(plane).GetRowSize(); }
  virtual int GetAlignedRowSize(Plane plane) const throw(NoSuchPlaneException) { return GetPlane(plane).GetAlignedRowSize(); }
  virtual int GetHeight(Plane plane) const throw(NoSuchPlaneException) { return GetPlane(plane).GetHeight(); }

  virtual const BYTE * GetReadPtr(Plane plane) const throw(NoSuchPlaneException) { return GetPlane(plane).GetReadPtr(); }
  virtual BYTE * GetWritePtr(Plane plane) throw(NoSuchPlaneException) { return GetPlane(plane).GetWritePtr(); }


  virtual const ColorSpace& GetColorSpace() const;
  virtual int GetVideoHeight() const;
  virtual int GetVideoWidth() const;

  virtual bool IsInterlaced() const;

  //morphing methods defined generally at this level
  //they test if plane exist and if yes process it.
  virtual void SizeChange(int left, int right, int top, int bottom);
  virtual void Copy(CPVideoFrame other, int left, int top);
  virtual void Blend(CPVideoFrame other, float factor);

  virtual CPProperty GetProperty(PPropertyKey key) const;
  virtual void SetProperty(CPProperty prop); 
  virtual void RemoveProperty(PPropertyKey key);

};





class InterleavedVideoFrame : public TaggedVideoFrame {

  typedef smart_ptr_to_cst<InterleavedVideoFrame> CPInterleavedVideoFrame;

protected:  
  BufferWindow main;

  virtual const BufferWindow& GetPlane(Plane plane) const throw(NoSuchPlaneException)
  {
    if (plane != NOT_PLANAR)
      throw NoSuchPlaneException();
    return main;
  }

  virtual BufferWindow& GetPlane(Plane plane) throw(NoSuchPlaneException)
  {
    if (plane != NOT_PLANAR)
      throw NoSuchPlaneException();
    return main;
  }


public:
  //copy constructor
  InterleavedVideoFrame(const InterleavedVideoFrame& other) : TaggedVideoFrame(other), main(other.main) { }
  //conversion constructor
  InterleavedVideoFrame(const ColorSpace& space, const TaggedVideoFrame& other) : TaggedVideoFrame(space, other) { }  
  //normal constructor
  InterleavedVideoFrame(const ColorSpace& space, int width, int height, bool IsField)
    : TaggedVideoFrame(space, width, height, IsField) { main.NewWindow(space.WidthToRowSize(width, NOT_PLANAR), height); }  

  virtual void FlipVertical() { main.FlipVertical(); }
};


class RGBVideoFrame : public InterleavedVideoFrame {

public:
  //copy constructor
  RGBVideoFrame(const RGBVideoFrame& other) : InterleavedVideoFrame(other) { }
  //conversion constructor
  RGBVideoFrame(const ColorSpace& space, const TaggedVideoFrame& other) : InterleavedVideoFrame(space, other) { }  
  //normal constructor
  RGBVideoFrame(const ColorSpace& space, int width, int height, bool IsField)
    : InterleavedVideoFrame(space, width, height, IsField) { }

  virtual void FlipHorizontal();

  virtual void TurnLeft(); 
  virtual void TurnRight();

};


class RGB32VideoFrame;
class YUY2VideoFrame;
class YV12VideoFrame;

class RGB24VideoFrame : public RGBVideoFrame {

  
protected:
  virtual RefCounted * clone() const { return new RGB24VideoFrame(*this); }  

public:
  RGB24VideoFrame(const RGB24VideoFrame& other) : RGBVideoFrame(other) { } 
  RGB24VideoFrame(int width, int height, bool IsField) : RGBVideoFrame(RGB24::instance, width, height, IsField) { }
  //converting constructors
  RGB24VideoFrame(const RGB32VideoFrame& other);
  RGB24VideoFrame(const YUY2VideoFrame& other);
  RGB24VideoFrame(const YV12VideoFrame& other);


  virtual CPVideoFrame ConvertTo(const ColorSpace& space) const;

};

class RGB32VideoFrame : public RGBVideoFrame {

protected:
  virtual RefCounted * clone() const { return new RGB32VideoFrame(*this); }

public:
  RGB32VideoFrame(const RGB32VideoFrame& other) : RGBVideoFrame(other) { } 
  RGB32VideoFrame(int width, int height, bool IsField) : RGBVideoFrame(RGB32::instance, width, height, IsField) { }
  //converting constructors
  RGB32VideoFrame(const RGB24VideoFrame& other);
  RGB32VideoFrame(const YUY2VideoFrame& other);
  RGB32VideoFrame(const YV12VideoFrame& other);

  virtual CPVideoFrame ConvertTo(const ColorSpace& space) const;

};


class YUY2VideoFrame : public InterleavedVideoFrame {
         
protected:
  virtual RefCounted * clone() const { return new YUY2VideoFrame(*this); }

  
public:
  YUY2VideoFrame(const RGB32VideoFrame& other);  //converting constructors
  YUY2VideoFrame(const RGB24VideoFrame& other);
  YUY2VideoFrame(const YV12VideoFrame& other);
  YUY2VideoFrame(const YUY2VideoFrame& other) : InterleavedVideoFrame(other) { } //spec of the above
  YUY2VideoFrame(int width, int height, bool IsField)
    : InterleavedVideoFrame(YUY2::instance, width, height, IsField) { }

  virtual CPVideoFrame ConvertTo(const ColorSpace& space) const;

  virtual void FlipHorizontal();

  virtual void TurnLeft(); 
  virtual void TurnRight();

};


/**********************************************************************************************/
/************************************ PlanarVideoFrame ****************************************/
/**********************************************************************************************/

class PlanarVideoFrame : public TaggedVideoFrame {

  typedef smart_ptr_to_cst<PlanarVideoFrame> CPPlanarVideoFrame;

protected:
  BufferWindow y, u ,v;

  virtual const BufferWindow& GetPlane(Plane plane) const throw(NoSuchPlaneException)
  {
    switch(plane) {
      case PLANAR_Y: return y;
      case PLANAR_U: return u;
      case PLANAR_V: return v;
    }
    throw NoSuchPlaneException();
  }

  virtual BufferWindow& GetPlane(Plane plane) throw(NoSuchPlaneException)
  {
    switch(plane) {
      case PLANAR_Y: return y;
      case PLANAR_U: return u;
      case PLANAR_V: return v;
    }
    throw NoSuchPlaneException();
  }

  virtual void InitPlanes(int width, int height) = 0;

public:
  //copy constructor
  PlanarVideoFrame(const PlanarVideoFrame& other) : TaggedVideoFrame(other), y(other.y), u(other.u), v(other.v) { }
  //normal constructor
  PlanarVideoFrame(const ColorSpace& space, int width, int height, bool IsField) 
    : TaggedVideoFrame(space, width, height, IsField) { InitPlanes(width, height); }
  

  virtual void Copy(CPVideoFrame other, int left, int top);

  virtual void FlipHorizontal();
  virtual void FlipVertical();

  virtual void TurnLeft(); 
  virtual void TurnRight();

  virtual void Blend(CPVideoFrame other, float factor);

  
}; 


class YV12VideoFrame : public PlanarVideoFrame {

protected:
  virtual void InitPlanes(int width, int height);
  virtual RefCounted * clone() const { return new YV12VideoFrame(*this); }

public:
  //copy constructor
  YV12VideoFrame(const YV12VideoFrame& other) : PlanarVideoFrame(other) { }
  //normal constructor
  YV12VideoFrame(int width, int height, bool IsField) : PlanarVideoFrame(YV12::instance, width, height, IsField) { }
  //conversions constructor
  YV12VideoFrame(const RGB24VideoFrame& other);
  YV12VideoFrame(const RGB32VideoFrame& other);
  YV12VideoFrame(const YUY2VideoFrame& other);

  virtual CPVideoFrame ConvertTo(const ColorSpace& space) const;

};

class VFWYV12VideoFrame : public YV12VideoFrame {

protected:
  virtual void InitPlanes(int width, int height);

public:
  VFWYV12VideoFrame(int width, int height, bool IsField) : YV12VideoFrame(width, height, IsField) { }

};

class VFWI420VideoFrame : public VFWYV12VideoFrame {
public:
  VFWI420VideoFrame(int width, int height, bool IsField) : VFWYV12VideoFrame(width, height, IsField) { std::swap(u, v); }
};


#endif  //ifndef __VIDEOFRAME_H__