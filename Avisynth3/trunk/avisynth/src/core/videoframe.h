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


#ifndef __AVS_VIDEOFRAME_H__
#define __AVS_VIDEOFRAME_H__

//avisynth includes
#include "plane.h"
#include "forward.h"
#include "property.h"
#include "frametype.h"
#include "window_ptr.h"

//stl include
#include <vector>


namespace avs {



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

  virtual CPVideoFrame clone() const = 0; 


public:  //general frame info
  
  //queries
  virtual FrameType GetType() const = 0;
  virtual ColorSpace& GetColorSpace() const = 0;
  virtual Dimension const& GetDimension() const = 0;

  //helper
  bool IsField() const { return GetType() < PROGRESSIVE; }
  bool IsFrame() const { return GetType() >= PROGRESSIVE; }

  bool MaybeInterlaced() const { return MaybeInterlaced(GetType()); }
  static bool MaybeInterlaced(FrameType type) { return type > PROGRESSIVE; }

  //write access
  virtual void SetType(FrameType type) = 0;


public:  //fetch environment method

  virtual PEnvironment const& GetEnvironment() const = 0;


public:  //plane data access

  virtual WindowPtr WriteTo(Plane plane);
  virtual CWindowPtr ReadFrom(Plane plane) const;


public:  //plane access (can do more with them than with the above)

  virtual BufferWindow& operator[](Plane plane) = 0;
  virtual BufferWindow const& operator[](Plane plane) const = 0;


public:  //property system

  typedef Property::PKey PKey;
  typedef std::vector<CPProperty> PropertyVector;

  virtual void SetProperty(CPProperty const& prop) = 0;
  //no effect if prop is not a static property
  virtual void SetStaticProperty(CPProperty const& prop) const = 0;

  virtual CPProperty GetStaticProperty(PKey const& key) const = 0;
  virtual void GetFlowProperties(PKey const& key, PropertyVector& propVector) const = 0;

  virtual void RemoveFlowProperties(PKey const& key) = 0;

};


} //namespace avs

#endif  //__AVS_VIDEOFRAME_H__
