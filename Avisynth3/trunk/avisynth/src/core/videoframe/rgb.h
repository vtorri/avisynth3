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


#ifndef __AVS_VIDEOFRAME_RGB_H__
#define __AVS_VIDEOFRAME_RGB_H__

//avisynth includes
#include "interleaved.h"
#include "../colorspace.h"


namespace avs { namespace vframe {



///////////////////////////////////////////////////////////////////////////////////////
//  RGB24
//
//  RGB24 specific VideoFrame subclass
//
class RGB24 : public Interleaved
{

public:  //structors

  //normal constructor
  RGB24(Dimension const& dim, FrameType type, PEnvironment const& env)
    : Interleaved( ColorSpace::rgb24(), dim, type, env ) { }

  //constructs using the given buffer
  RGB24(Dimension const& dim, FrameType type, BufferWindow const& main)
    : Interleaved( ColorSpace::rgb24(), dim, type, main ) { }

  //generated copy constructor and destructor are fine


public:  //clone method

  virtual CPVideoFrame clone() const { return CPVideoFrame( static_cast<VideoFrame *>(new RGB24(*this)) ); }


public:  //general frame info

  virtual ColorSpace& GetColorSpace() const { return ColorSpace::rgb24(); }


};



///////////////////////////////////////////////////////////////////////////////////////
//  RGB32
//
//  RGB32 specific VideoFrame subclass
//
class RGB32 : public Interleaved
{

public:  //structors

  //normal constructor
  RGB32(Dimension const& dim, FrameType type, PEnvironment const& env)
    : Interleaved( ColorSpace::rgb32(), dim, type, env ) { }

  //constructs using the given buffer
  RGB32(Dimension const& dim, FrameType type, BufferWindow const& main)
    : Interleaved( ColorSpace::rgb32(), dim, type, main ) { }

  //generated copy constructor and destructor are fine


public:  //clone method

  virtual CPVideoFrame clone() const { return CPVideoFrame( static_cast<VideoFrame *>(new RGB32(*this)) ); }


public:  //general frame info

  virtual ColorSpace& GetColorSpace() const { return ColorSpace::rgb32(); }

};



///////////////////////////////////////////////////////////////////////////////////////
//  RGB45
//
//  RGB45 specific VideoFrame subclass
//
class RGB45 : public Interleaved
{

public:  //structors

  //normal constructor
  RGB45(Dimension const& dim, FrameType type, PEnvironment const& env)
    : Interleaved( ColorSpace::rgb45(), dim, type, env ) { }

  //constructs using the given buffer
  RGB45(Dimension const& dim, FrameType type, BufferWindow const& main)
    : Interleaved( ColorSpace::rgb45(), dim, type, main ) { }

  //generated copy constructor and destructor are fine


public:  //clone method

  virtual CPVideoFrame clone() const { return CPVideoFrame( static_cast<VideoFrame *>(new RGB45(*this)) ); }


public:  //general frame info

  virtual ColorSpace& GetColorSpace() const { return ColorSpace::rgb45(); }

};



} } //namespace avs::vframe

#endif //__AVS_VIDEOFRAME_RGB_H__
