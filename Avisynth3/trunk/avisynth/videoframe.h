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

  void InitPlanes(int align);
  
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
  virtual void FlipVertical();
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

  virtual void PlaneInit(int align)
  {
    const ColorSpace& space = GetColorSpace();
    GetPlane(NOT_PLANAR) = BufferWindow(space.WidthToRowSize(GetVideoWidth(), NOT_PLANAR), space.HeightToPlaneHeight(GetVideoHeight(), NOT_PLANAR), align);
  }

public:
  InterleavedVideoFrame(const InterleavedVideoFrame& other) : TaggedVideoFrame(other), main(other.main) { }
  InterleavedVideoFrame(const ColorSpace& space, int width, int height, bool IsField, int align)
    : TaggedVideoFrame(space, width, height, IsField) { PlaneInit(align); }  
  

};


class RGBVideoFrame : public InterleavedVideoFrame {

public:
  RGBVideoFrame(const RGBVideoFrame& other) : InterleavedVideoFrame(other) { }
  RGBVideoFrame(const ColorSpace& space, int width, int height, bool IsField, int align)
    : InterleavedVideoFrame(space, width, height, IsField, align) { }

  virtual void FlipHorizontal();

  virtual void TurnLeft(); 
  virtual void TurnRight();

};


class RGB32VideoFrame;
class YUY2VideoFrame;
class PlanarVideoFrame;

class RGB24VideoFrame : public RGBVideoFrame {

  
protected:
  virtual RefCounted * clone() const { return new RGB24VideoFrame(*this); }  

public:
  RGB24VideoFrame(const RGB32VideoFrame& other);  //converting constrcutors
  RGB24VideoFrame(const YUY2VideoFrame& other);
  RGB24VideoFrame(const PlanarVideoFrame& other);
  RGB24VideoFrame(const RGB24VideoFrame& other) : RGBVideoFrame(other) { } 
  RGB24VideoFrame(int width, int height, bool IsField, int align)
    : RGBVideoFrame(RGB24::instance, width, height, IsField, align) { }

  virtual CPVideoFrame ConvertTo(const ColorSpace& space, int align = FRAME_ALIGN) const;

};

class RGB32VideoFrame : public RGBVideoFrame {

protected:
  virtual RefCounted * clone() const { return new RGB32VideoFrame(*this); }

public:
  RGB32VideoFrame(const RGB24VideoFrame& other);  //converting constrcutors
  RGB32VideoFrame(const YUY2VideoFrame& other);
  RGB32VideoFrame(const PlanarVideoFrame& other);
  RGB32VideoFrame(const RGB32VideoFrame& other) : RGBVideoFrame(other) { } 
  RGB32VideoFrame(int width, int height, bool IsField, int align)
    : RGBVideoFrame(RGB32::instance, width, height, IsField, align) { }


  virtual CPVideoFrame ConvertTo(const ColorSpace& space, int align = FRAME_ALIGN) const;

};


class YUY2VideoFrame : public InterleavedVideoFrame {
         
protected:
  virtual RefCounted * clone() const { return new YUY2VideoFrame(*this); }

  
public:
  YUY2VideoFrame(const RGB32VideoFrame& other);  //converting constructors
  YUY2VideoFrame(const RGB24VideoFrame& other);
  YUY2VideoFrame(const PlanarVideoFrame& other);
  YUY2VideoFrame(const YUY2VideoFrame& other) : InterleavedVideoFrame(other) { } //spec of the above
  YUY2VideoFrame(int width, int height, bool IsField, int align)
    : InterleavedVideoFrame(YUY2::instance, width, height, IsField, align) { }

  virtual CPVideoFrame ConvertTo(const ColorSpace& space, int align = FRAME_ALIGN) const;

  virtual void FlipHorizontal();

  virtual void TurnLeft(); 
  virtual void TurnRight();

};


/**********************************************************************************************/
/************************************ PlanarVideoFrame ****************************************/
/**********************************************************************************************/

class PlanarVideoFrame : public TaggedVideoFrame {

  typedef smart_ptr_to_cst<PlanarVideoFrame> CPPlanarVideoFrame;

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

protected:
  virtual RefCounted * clone() const { return new PlanarVideoFrame(*this); }

public:
  PlanarVideoFrame(const RGB32VideoFrame& other);  //converting constructors
  PlanarVideoFrame(const RGB24VideoFrame& other);
  PlanarVideoFrame(const YUY2VideoFrame& other);
  PlanarVideoFrame(const PlanarVideoFrame& other) : TaggedVideoFrame(other), y(other.y), u(other.u), v(other.v) { }
  PlanarVideoFrame(int width, int height, bool IsField, int align) 
    : TaggedVideoFrame(YV12::instance, width, height, IsField) { InitPlanes(align); }
  

  virtual CPVideoFrame ConvertTo(const ColorSpace& space, int align = FRAME_ALIGN) const;

  virtual void Copy(CPVideoFrame other, int left, int top);

  virtual void FlipHorizontal();
  virtual void FlipVertical();

  virtual void TurnLeft(); 
  virtual void TurnRight();

  virtual void Blend(CPVideoFrame other, float factor);

  
}; 














#endif  //ifndef __VIDEOFRAME_H__