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



#include "videoinfo.h"
#include "bufferwindow.h"
#include "property.h" 


//typedef smart_ptr<FrameVideoProperties> PFrameVidProps;




class PlaneMetrics {

  Dimension dimension;
  int pitch;

public:
  PlaneMetrics() { }
  PlaneMetrics(const BufferWindow& bw) : dimension(bw.GetDimension()), pitch(bw.GetPitch()) { }
  PlaneMetrics(const PlaneMetrics& other) : dimension(other.dimension), pitch(other.pitch) { }

  const PlaneMetrics& operator=(const PlaneMetrics& other) { dimension = other.dimension; pitch = other.pitch; return *this; }

  int GetRowSize() const { return dimension.GetWidth(); }
  int GetAlignedRowSize() const { int r = ( GetRowSize() + FRAME_ALIGN - 1 ) & -FRAME_ALIGN; return r <= pitch ? r : GetRowSize(); }
  int GetHeight() const { return dimension.GetHeight(); }
  int GetPitch() const { return pitch; }
};



/********************************************************************************************
 * Polymorphic VideoFrame class                                                             *
 ********************************************************************************************
  VideoFrameBuffer no longer defined there (in videoframe.h)
  VideoFrame still use them, but they are hidden by the VideoFrame API

  MakeWritable is no longer required : CPVideoFrame are never Writable, nor mutable in any sort
  PVideoframe are always writable/mutable, and will copy buffers before returning Write Ptr if they are shared
*/



class VideoFrame;
typedef smart_ptr<VideoFrame> PVideoFrame;
typedef smart_ptr_to_cst<VideoFrame> CPVideoFrame;


class VideoFrame : public RefCounted {

  PFrameVidProps vidProps;
  PropertySet propSet;

public:
  VideoFrame(const FrameVideoProperties& _vidProps)
    : vidProps(const_cast<FrameVideoProperties &>(_vidProps)) { }
  //copy constructor
  VideoFrame(const VideoFrame& other) : vidProps(other.vidProps), propSet(other.propSet)  { }

  //clone method, expected by smart_ptrs for PVideoFrame/CPVideoFrame conversions
  virtual VideoFrame * clone() const = 0; 


  //static method to convert index in Plane enum, USE it !!!
  //the NOT_PLANAR plane is provided in last position
  //for cases where you process all planes in the same way (use with ColorSpace::HasPlane(Plane) )
  inline static Plane IndexToPlane(int i) {
    static Plane planes[] = { PLANAR_Y, PLANAR_U, PLANAR_V, NOT_PLANAR };
    return planes[i];
  }


  PlaneMetrics GetMetrics(Plane plane) const throw(invalid_plane) { return PlaneMetrics(GetPlane(plane)); }

  const BYTE * GetReadPtr(Plane plane) const throw(invalid_plane) { return GetPlane(plane).GetReadPtr(); }
  BYTE * GetWritePtr(Plane plane) throw(invalid_plane) { return GetPlane(plane).GetWritePtr(); }

  
  //methods to get some other general infos
  const ColorSpace& GetColorSpace() const { return vidProps->GetColorSpace(); }
  const Dimension& GetDimension() const { return vidProps->GetDimension(); }
  FrameType GetFrameType() const { return vidProps->GetFrameType(); }

  int GetWidth() const { return GetDimension().GetWidth(); }
  int GetHeight() const { return GetDimension().GetHeight(); }

  bool IsPlanar() const { return GetColorSpace().HasProperty(ColorSpace::PLANAR); }  

  bool IsField() const { return vidProps->IsField(); }
  bool IsFrame() const { return vidProps->IsFrame(); }



  /************************************************************************************
   *  various ToolBox methods now provided by the VideoFrame API                      *
   ************************************************************************************/

  //ColorSpace conversion method
  virtual CPVideoFrame ConvertTo(const ColorSpace& space) const = 0;

  //Please Note that shift vecteurs are in PIXELS
  //buffers won't be reallocated if possible
  //if reallocation has to be done, original data is copied at the right place
  //new data, if there is any, is not initialized 
  void SizeChange(const Vecteur& topLeft, const Vecteur& bottomRight);

  //Copy other into self at the specified coords (in Pixels)
  //Only the overlapping part is copied
  void Copy(const VideoFrame& other, const Vecteur& coords);

  void StackHorizontal(const VideoFrame& other);  //both implemented using SizeChange and Copy       
  void StackVertical(const VideoFrame& other);
  
  virtual void FlipHorizontal() = 0;
  virtual void FlipVertical() = 0;

  virtual void TurnLeft() = 0;  //how do we turn a YUY2 frame !?...
  virtual void TurnRight() = 0;

  virtual void Blend(const VideoFrame& other, float factor) = 0;

  //what else ???

  /************************************************************************************
   *  properties methods                                                              *
   ************************************************************************************/

  CPProperty GetProperty(const PropertyKey& key) const { propSet.Get(key); }

  void SetProperty(const Property& prop) { propSet.Set(prop); }
  void RemoveProperty(const PropertyKey& key) { propSet.Remove(key); }



protected:
  //plane fetch methods (must be consistent together)
  //their existence allow to define many things at this level
  virtual const BufferWindow& GetPlane(Plane plane) const throw(invalid_plane) = 0;
  virtual BufferWindow& GetPlane(Plane pane) throw(invalid_plane) = 0;


};









class InterleavedVideoFrame : public VideoFrame {

protected:  
  BufferWindow main;

  virtual const BufferWindow& GetPlane(Plane plane) const throw(invalid_plane)
  {
    if (plane != NOT_PLANAR)
      throw invalid_plane();
    return main;
  }

  virtual BufferWindow& GetPlane(Plane plane) throw(invalid_plane)
  {
    if (plane != NOT_PLANAR)
      throw invalid_plane();
    return main;
  }

public:
  InterleavedVideoFrame(const FrameVideoProperties& vidProps) : VideoFrame(vidProps), main(vidProps, NOT_PLANAR) { }
  //copy constructor
  InterleavedVideoFrame(const InterleavedVideoFrame& other) : VideoFrame(other), main(other.main) { }

  virtual void FlipHorizontal() { main.FlipHorizontal(static_cast<const InterLeaved&>(GetColorSpace()).GetBytesPerPixel()); }
  virtual void FlipVertical() { main.FlipVertical(); }

  virtual void TurnLeft(); 
  virtual void TurnRight();

  virtual void Blend(const VideoFrame& other, float factor);
};



class RGBVideoFrame : public InterleavedVideoFrame {

  //normal constructor, private to avoid construction with illegal colorspace props (unchecked)
  RGBVideoFrame(const FrameVideoProperties& vidProps) : InterleavedVideoFrame(vidProps) { }

public:
  //copy constructor
  RGBVideoFrame(const RGBVideoFrame& other) : InterleavedVideoFrame(other) { } 

  virtual VideoFrame * clone() const { return new RGBVideoFrame(*this); }  

  virtual CPVideoFrame ConvertTo(const ColorSpace& space) const;

};




class YUY2VideoFrame : public InterleavedVideoFrame {
         
  YUY2VideoFrame(const FrameVideoProperties& vidProps) : InterleavedVideoFrame(vidProps) { }
  
public:
  //copy constructor
  YUY2VideoFrame(const YUY2VideoFrame& other) : InterleavedVideoFrame(other) { } 

  virtual VideoFrame * clone() const { return new YUY2VideoFrame(*this); }

  virtual CPVideoFrame ConvertTo(const ColorSpace& space) const;

  virtual void FlipHorizontal();

  virtual void TurnLeft(); 
  virtual void TurnRight();

};


/**********************************************************************************************/
/************************************ PlanarVideoFrame ****************************************/
/**********************************************************************************************/

class PlanarVideoFrame : public VideoFrame {

protected:
  BufferWindow y, u ,v;

  virtual const BufferWindow& GetPlane(Plane plane) const throw(invalid_plane)
  {
    switch(plane) {
      case PLANAR_Y: return y;
      case PLANAR_U: return u;
      case PLANAR_V: return v;
    }
    throw invalid_plane();
  }

  virtual BufferWindow& GetPlane(Plane plane) throw(invalid_plane)
  {
    switch(plane) {
      case PLANAR_Y: return y;
      case PLANAR_U: return u;
      case PLANAR_V: return v;
    }
    throw invalid_plane();
  }

  //normal constructor
  PlanarVideoFrame(const FrameVideoProperties& vidProps) : VideoFrame(vidProps),
    y(vidProps, PLANAR_Y), u(vidProps, PLANAR_U), v(vidProps, PLANAR_V) { }

public:
  //copy constructor
  PlanarVideoFrame(const PlanarVideoFrame& other) : VideoFrame(other), y(other.y), u(other.u), v(other.v) { }

  


  virtual void FlipHorizontal();
  virtual void FlipVertical();

  virtual void TurnLeft(); 
  virtual void TurnRight();

  virtual void Blend(const VideoFrame& other, float factor);

  
}; 


class YV12VideoFrame : public PlanarVideoFrame {

protected:
  //normal constructor
  YV12VideoFrame(const FrameVideoProperties& vidProps) : PlanarVideoFrame(vidProps) { }

public:
  //copy constructor
  YV12VideoFrame(const YV12VideoFrame& other) : PlanarVideoFrame(other) { }

  virtual VideoFrame * clone() const { return new YV12VideoFrame(*this); }

  virtual CPVideoFrame ConvertTo(const ColorSpace& space) const;

};

class VFWYV12VideoFrame : public YV12VideoFrame {

  VFWYV12VideoFrame(const FrameVideoProperties& vidProps) : YV12VideoFrame(vidProps) { }

};

class VFWI420VideoFrame : public YV12VideoFrame {

  VFWI420VideoFrame(const FrameVideoProperties& vidProps) : YV12VideoFrame(vidProps) { std::swap(u, v); }
};


#endif  //ifndef __VIDEOFRAME_H__