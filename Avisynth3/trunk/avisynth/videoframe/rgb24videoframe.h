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


#ifndef __AVS_RGB24VIDEOFRAME_H__
#define __AVS_RGB24VIDEOFRAME_H__


//avisynth include
#include "rgbvideoframe.h"


namespace avs {


//class declarations
class YUY2VideoFrame;
class YV12VideoFrame;
class RGB32VideoFrame;



///////////////////////////////////////////////////////////////////////////////////////
//  RGB24VideoFrame
//
//
//
class RGB24VideoFrame : public RGBVideoFrame
{

public:  //constructors

  //normal constructor
  RGB24VideoFrame(const Dimension dim, FrameType type, PEnvironment env);

  //copy constructor
  RGB24VideoFrame(const RGB24VideoFrame& other)
    : RGBVideoFrame( other ) { }

  //conversion from RGB32
  explicit RGB24VideoFrame(const RGB32VideoFrame other);

  //conversion from YUY2
  explicit RGB24VideoFrame(const YUY2VideoFrame other);

  //conversion from YV12
  explicit RGB24VideoFrame(const YV12VideoFrame other);


public:  //clone methods

  virtual VideoFrame * clone() const { return new RGB24VideoFrame(*this); }

  virtual VideoFrame * BlankClone() const;


public:  //general frame info

  virtual ColorSpace& GetColorSpace() const;


public:  //toolbox methods

  virtual CPVideoFrame ConvertTo(const ColorSpace& space) const;


public:  //helper method

  virtual int BytesPerPixel() const { return 3; }

};




}; //namespace avs

#endif //__AVS_RGB24VIDEOFRAME_H__