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


#ifndef __AVS_RGBVIDEOFRAME_H__
#define __AVS_RGBVIDEOFRAME_H__


#include "interleavedvideoframe.h"


namespace avs {


///////////////////////////////////////////////////////////////////////////////////////
//  RGBVideoFrame
//
//  Exists only for the sake of providing RGB24 & 32 VideoFrame
//  with a common superclass, thus mirroring the ColorSpace hierarchy
//
class RGBVideoFrame : public InterleavedVideoFrame
{

public:  //constructors

  //normal constructor
  RGBVideoFrame(ColorSpace& space, const Dimension dim, FrameType type, PEnvironment env)
    : InterleavedVideoFrame( space, dim, type, env ) { }
 
  //conversion constructor
  RGBVideoFrame(ColorSpace& space, const AbstractVideoFrame& other)
    : InterleavedVideoFrame( space, other ) { }

  //copy constructor
  RGBVideoFrame(const RGBVideoFrame& other)
    : InterleavedVideoFrame( other ) { } 


public:  //toolbox methods

  virtual void FlipHorizontal() { main.FlipHorizontal(BytesPerPixel()); }

  virtual void TurnLeft() { main.TurnLeft(BytesPerPixel()); }
  virtual void TurnRight() { main.TurnRight(BytesPerPixel()); }


public:  //helper method

  virtual int BytesPerPixel() const = 0;

};




}; //namespace avs

#endif //__AVS_RGBVIDEOFRAME_H__