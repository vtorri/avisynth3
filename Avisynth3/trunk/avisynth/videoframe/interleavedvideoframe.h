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


#ifndef __AVS_INTERLEAVEDVIDEOFRAME_H__
#define __AVS_INTERLEAVEDVIDEOFRAME_H__


#include "abstractvideoframe.h"
#include "../bufferwindow.h"


namespace avs {



////////////////////////////////////////////////////////////////////////////////////
// InterleavedVideoFrame
//
// videoframe subclass for interleaved colorspaces (YUY2, RGB24, RGB32)
// where all the data is in only one frame buffer
//
class InterleavedVideoFrame : public AbstractVideoFrame
{

protected:

  BufferWindow main;


public:  //constructors

  //normal constructor
  InterleavedVideoFrame(ColorSpace& space, const Dimension& dim, FrameType type, PEnvironment env)
    : AbstractVideoFrame( space, dim, type )
    , main( space, dim, NOT_PLANAR, env) { }

  //conversion constructor
  InterleavedVideoFrame(ColorSpace& space, const AbstractVideoFrame& other)
    : AbstractVideoFrame( space, other )
    , main( space, other.GetDimension(), NOT_PLANAR, other.GetEnvironment() ) { }

  //copy constructor
  InterleavedVideoFrame(const InterleavedVideoFrame& other)
    : AbstractVideoFrame( other )
    , main( other.main ) { }


public:  //fetch environment method

  virtual PEnvironment GetEnvironment() const { return main.GetEnvironment(); }


public:  //plane access

  virtual const BufferWindow& operator[](Plane plane) const
  {
    if ( plane != NOT_PLANAR )
      ThrowInvalidPlaneException( GetColorSpace(), plane );
    return main;
  }

  virtual BufferWindow& operator[](Plane plane)
  {
    if ( plane != NOT_PLANAR )
      ThrowInvalidPlaneException( GetColorSpace(), plane );
    return main;
  }

  const BufferWindow& GetMain() const { return main; }
  BufferWindow& GetMain() { return main; }


public:  //toolbox methods

  virtual void FlipVertical() { main.FlipVertical(); }

  virtual void Blend(CPVideoFrame other, float factor);

};





}; //namespace avs

#endif //__AVS_INTERLEAVEDVIDEOFRAME_H__