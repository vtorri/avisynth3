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


#ifndef __AVS_RGB32VIDEOFRAME_H__
#define __AVS_RGB32VIDEOFRAME_H__


//avisynth include
#include "rgbvideoframe.h"


namespace avs {


//class declarations
class YUY2VideoFrame;
class YV12VideoFrame;
class RGB24VideoFrame;



///////////////////////////////////////////////////////////////////////////////////////
//  RGB32VideoFrame
//
//
//
class RGB32VideoFrame : public RGBVideoFrame
{

public:  //constructors

  //normal constructor
  RGB32VideoFrame(const Dimension dim, FrameType type, PEnvironment env)
    : RGBVideoFrame( vidProps, env ) { }

  //copy constructor
  RGB32VideoFrame(const RGB32VideoFrame& other)
    : RGBVideoFrame( other ) { }

  //conversion from RGB24
  explicit RGB32VideoFrame(const RGB24VideoFrame& other);

  //conversion from YUY2
  explicit RGB32VideoFrame(const YUY2VideoFrame& other);

  //conversion from YV12
  explicit RGB32VideoFrame(const YV12VideoFrame& other);


public:  //clone methods

  virtual VideoFrame * clone() const { return new RGB32VideoFrame(*this); }

  virtual VideoFrame * BlankClone() const;


public:  //toolbox methods

  virtual CPVideoFrame ConvertTo(const ColorSpace& space) const;


public:  //helper method

  virtual int BytesPerPixel() const { return 4; }

};



}; //namespace avs

#endif //__AVS_RGB32VIDEOFRAME_H__