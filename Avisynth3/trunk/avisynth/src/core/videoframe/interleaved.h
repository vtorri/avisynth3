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


#ifndef __AVS_VIDEOFRAME_INTERLEAVED_H__
#define __AVS_VIDEOFRAME_INTERLEAVED_H__

//avisynth includes
#include "base.h"
#include "../colorspace.h"
#include "../bufferwindow.h"
#include "../exception/nosuchplane.h"

//assert include
#include "assert.h"


namespace avs { namespace vframe {



//////////////////////////////////////////////////////////////////////////////////////////////
//  Interleaved
//
//  VideoFrame factorisation superclass for interleaved colorspaces (YUY2, RGB24, RGB32, RGB45)
//  where all the data is in only one frame buffer
//
class Interleaved : public Base
{
  
  BufferWindow main_;


public:  //structors

  //normal constructor
  Interleaved(ColorSpace& space, Dimension const& dim, FrameType type, PEnvironment const& env)
    : Base( space, dim, type )
    , main_( space.ToPlaneDim(dim, NOT_PLANAR), env ) { }

  //constructs using the given buffer
  Interleaved(ColorSpace& space, Dimension const& dim, FrameType type, BufferWindow const& main)
    : Base( space, dim, type )
    , main_( main )
  {
    assert( main.GetDimension() == space.ToPlaneDim(dim, NOT_PLANAR) );   //check the given buffer fits
  }

  //generated copy constructor and destructor are fine


public:  //fetch environment method

  virtual PEnvironment const& GetEnvironment() const { return main_.GetEnvironment(); }


public:  //plane access

  virtual BufferWindow& operator[](Plane plane)
  {
    if ( plane != NOT_PLANAR )
      throw exception::NoSuchPlane(GetColorSpace(), plane);
    return GetMain();
  }

  virtual const BufferWindow& operator[](Plane plane) const
  {
    if ( plane != NOT_PLANAR )
      throw exception::NoSuchPlane(GetColorSpace(), plane);
    return GetConstMain();
  }

  BufferWindow& GetMain() { ClearStaticProperties(); return main_; }
  BufferWindow const& GetConstMain() const { return main_; }

};



} } //namespace avs::vframe

#endif //__AVS_VIDEOFRAME_INTERLEAVED_H__
