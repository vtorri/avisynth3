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


#ifndef __AVS_VIDEOFRAME_INTERLEAVED_H__
#define __AVS_VIDEOFRAME_INTERLEAVED_H__


//avisynth includes
#include "vframe_base.h"
#include "../colorspace.h"
#include "../bufferwindow.h"


namespace avs {



////////////////////////////////////////////////////////////////////////////////////
// VideoFrame::Interleaved
//
// factorisation superclass for interleaved colorspaces (YUY2, RGB24, RGB32, RGB45)
// where all the data is in only one frame buffer
//
class VideoFrame::Interleaved : public VideoFrame::Base
{
  
  BufferWindow main;


public:  //constructors

  //normal constructor
  Interleaved(ColorSpace& space, Dimension const& dim, FrameType type, PEnvironment env)
    : Base( space, dim, type )
    , main( ToPlaneDimension(space, dim, NOT_PLANAR), env ) { }

  //conversion constructor
  Interleaved(ColorSpace& space, Base const& other)
    : Base( space, other )
    , main( ToPlaneDimension(space, other.GetDimension(), NOT_PLANAR), other.GetEnvironment() ) { }

  //copy constructor
  Interleaved(Interleaved const& other)
    : Base( other )
    , main( other.main ) { }


public:  //fetch environment method

  virtual PEnvironment GetEnvironment() const { return main.GetEnvironment(); }


public:  //plane access

  virtual BufferWindow& operator[](Plane plane)
  {
    if ( plane != NOT_PLANAR )
      GetColorSpace().ThrowInvalidPlaneException(plane);
    return GetMain();
  }

  virtual const BufferWindow& operator[](Plane plane) const
  {
    if ( plane != NOT_PLANAR )
      GetColorSpace().ThrowInvalidPlaneException(plane);
    return GetMain();
  }

  BufferWindow& GetMain() { return main; }
  BufferWindow const& GetMain() const { return main; }


public:  //toolbox methods

  virtual void SizeChange(Vecteur const& topLeft, Vecteur const& bottomRight);

  virtual void Copy(CPVideoFrame other, Vecteur const& coords);

  virtual void FlipVertical() { main.FlipVertical(); }
  virtual void FlipHorizontal() { main.FlipHorizontal( BytesPerPixel() ); }

  virtual void TurnLeft() { main.TurnLeft( BytesPerPixel() ); }
  virtual void TurnRight() { main.TurnRight( BytesPerPixel() ); }

  virtual void Blend(CPVideoFrame other, float factor);


public:  //BytesPerPixel helper method

  int BytesPerPixel() const;

};//VideoFrame::Interleaved





} //namespace avs

#endif //__AVS_VIDEOFRAME_INTERLEAVED_H__