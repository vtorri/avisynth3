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
#include "frametype.h"
#include "smart_ref.h"

//stl include
#include <vector>


//class declarations
struct Vecteur;
class Property;
class Dimension;
class VideoFrame;                                     //defined here
class ColorSpace;
class PropertyKey;
class BufferWindow;
class FrameVideoProperties;

//typedefs
typedef smart_ref<const Property> CPProperty;
typedef std::vector<CPProperty> PropertyVector;
typedef smart_ref<const VideoFrame> CPVideoFrame;     //const VideoFrame *
typedef smart_ref<const FrameVideoProperties> CPFrameVidProps;



///////////////////////////////////////////////////////////////////////////////////////////
//  VideoFrame
//
//  abstract base class of the VideoFrame hierarchy
//
class VideoFrame
{

  CPFrameVidProps vidProps_;
  PropertyVector propVector;


public:  //constructors

  //normal constructor
  VideoFrame(CPFrameVidProps vidProps)
    : vidProps_( vidProps ) { }
  
  //copy constructor
  VideoFrame(const VideoFrame& other)
    : vidProps_( other.vidProps_ )
    , propVector( other.propVector )  { }


public:  //clone method (expected by smart_ptrs for PVideoFrame/CPVideoFrame conversions)

  virtual VideoFrame * clone() const = 0; 


public:  //plane access (see BufferWindow)

  virtual const BufferWindow& operator[](Plane plane) const = 0;
  virtual BufferWindow& operator[](Plane plane) = 0;


public:  //general info (about the whole frame)
  
  const ColorSpace& GetColorSpace() const;
  const Dimension& GetDimension() const;

  FrameType GetFrameType() const;

  bool IsField() const { return GetFrameType() < PROGRESSIVE; }
  bool IsFrame() const { return GetFrameType() >= PROGRESSIVE; }


  void SetFrameType(FrameType type);


public:  //toolbox methods

  //ColorSpace conversion method
  virtual CPVideoFrame ConvertTo(const ColorSpace& space) const = 0;

  //Please Note that shift vecteurs are in PIXELS
  //buffers won't be reallocated if possible
  //if reallocation has to be done, original data is copied at the right place
  //new data, if there is any, is not initialized 
  void SizeChange(const Vecteur& topLeft, const Vecteur& bottomRight);

  //Copy other into self at the specified coords (in Pixels)
  //Only the overlapping part is copied
  void Copy(CPVideoFrame other, const Vecteur& coords);

  void StackHorizontal(CPVideoFrame other);  //both implemented using SizeChange and Copy       
  void StackVertical(CPVideoFrame other);
  
  virtual void FlipHorizontal() = 0;
  virtual void FlipVertical() = 0;

  virtual void TurnLeft() = 0;  //how do we turn a YUY2 frame !?...
  virtual void TurnRight() = 0;

  virtual void Blend(CPVideoFrame other, float factor) = 0;


public:  //property system

  void SetProperty(CPProperty prop);
  void RemoveProperty(const PropertyKey& key);

  boost::shared_ptr<const Property> GetProperty(const PropertyKey& key) const;

};






#endif  //__AVS_VIDEOFRAME_H__