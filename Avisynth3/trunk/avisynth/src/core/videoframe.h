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


#ifndef __AVS_VIDEOFRAME_H__
#define __AVS_VIDEOFRAME_H__

//avisynth includes
#include "plane.h"
#include "property.h"
#include "window_ptr.h"

//boost include
#include <boost/shared_ptr.hpp>


namespace avs {


//class declarations
struct Vecteur;
class Dimension;
class VideoFrame;                                     //defined here
class ColorSpace;
class BufferWindow;
class RuntimeEnvironment;


//typedefs
typedef boost::shared_ptr<VideoFrame const> CPVideoFrame;
typedef boost::shared_ptr<RuntimeEnvironment> PEnvironment;


///////////////////////////////////////////////////////////////////////////////////////////
//  VideoFrame
//
//  abstract base class of the VideoFrame hierarchy
//
class VideoFrame
{

public:  //structors

  VideoFrame() { }
  virtual ~VideoFrame() { }


public:  //clone method 

  virtual VideoFrame * clone() const = 0; 


public:  //Type enum

  enum Type
  {
    FIELD_TOP,
    FIELD_BOTTOM,
    PROGRESSIVE,
    INTERLACED_TFF,
    INTERLACED_BFF,
    UNKNOWN  
  };


public:  //general frame info
  
  virtual ColorSpace& GetColorSpace() const = 0;

  virtual Dimension const& GetDimension() const = 0;
  virtual Type GetType() const = 0;

  virtual void SetType(Type type);

  bool IsField() const { return GetType() < PROGRESSIVE; }
  bool IsFrame() const { return GetType() >= PROGRESSIVE; }


  static bool MaybeInterlaced(Type type) { return type > PROGRESSIVE; }

  bool MaybeInterlaced() const { return MaybeInterlaced(GetType()); }


public:  //fetch environment method

  virtual PEnvironment GetEnvironment() const = 0;


public:  //plane data access

  virtual WindowPtr WriteTo(Plane plane) = 0;
  virtual CWindowPtr ReadFrom(Plane plane) const = 0;


public:  //plane access (can do more with them than with the above)

  virtual BufferWindow& operator[](Plane plane) = 0;
  virtual BufferWindow const& operator[](Plane plane) const = 0;


public:  //toolbox methods

  //ColorSpace conversion method
  virtual CPVideoFrame ConvertTo(ColorSpace& space) const = 0;

  //Please Note that shift vecteurs are in PIXELS
  //buffers won't be reallocated if possible
  //if reallocation has to be done, original data is copied at the right place
  //new data, if there is any, is not initialized 
  virtual void SizeChange(Vecteur const& topLeft, Vecteur const& bottomRight) = 0;

  //Copy other into self at the specified coords (in Pixels)
  //Only the overlapping part is copied
  virtual void Copy(CPVideoFrame other, Vecteur const& coords) = 0;

  
  virtual void FlipVertical() = 0;
  virtual void FlipHorizontal() = 0;

  virtual void TurnLeft() = 0;  //how do we turn a YUY2 frame !?...
  virtual void TurnRight() = 0;

  virtual void Blend(CPVideoFrame other, float factor) = 0;


public:  //property system

  virtual void SetProperty(CPProperty prop) = 0;
  virtual void RemoveProperty(Property::Key const& key) = 0;

  virtual CPProperty GetProperty(Property::Key const& key) const = 0;



public:  //inner implementation subclasses

  class Base;          //: public VideoFrame

  class Interleaved;   //: public Base

  class RGB;           //: public Interleaved
  class RGB24;         //: public RGB
  class RGB32;         //: public RGB
  class RGB45;         //: public RGB

  class YUY2;          //: public Interleaved

  struct Planar
  {
    class YUV;         //: public Base
  };

  class YV12;          //: public Planar::YUV
  class YV24;          //: public Planar::YUV
  class YV45;          //: public Planar::YUV

};//VideoFrame




} //namespace avs

#endif  //__AVS_VIDEOFRAME_H__