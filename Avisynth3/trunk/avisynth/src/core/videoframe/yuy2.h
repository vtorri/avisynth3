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


#ifndef __AVS_VIDEOFRAME_YUY2_H__
#define __AVS_VIDEOFRAME_YUY2_H__

//avisynth includes
#include "interleaved.h"
#include "../colorspace.h"


namespace avs { namespace vframe {



////////////////////////////////////////////////////////////////////////////////
//  YUY2
//
//  YUY2 specific VideoFrame subclass
//
class YUY2 : public Interleaved
{
           
public:  //structors

  //normal constructor
  YUY2(Dimension const& dim, FrameType type, PEnvironment env)
    : Interleaved( ColorSpace::yuy2(), dim, type, env ) { }

  //constructs using the given buffer
  YUY2(Dimension const& dim, FrameType type, BufferWindow const& main)
    : Interleaved( ColorSpace::yuy2(), dim, type, main ) { }

  //generated copy constructor and destructor are fine


public:  //clone method

  virtual CPVideoFrame clone() const { return CPVideoFrame( static_cast<VideoFrame *>(new YUY2(*this)) ); }


public:  //general frame info

  virtual ColorSpace& GetColorSpace() const { return ColorSpace::yuy2(); }

};



} } //namespace avs::vframe

#endif //__AVS_VIDEOFRAME_YUY2_H__
