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
#include "../bufferwindow.h"
#include "../bufferwindow/blender.h"
#include "../bufferwindow/leftturner.h"
#include "../bufferwindow/rightturner.h"
#include "../bufferwindow/verticalflipper.h"
#include "../bufferwindow/horizontalflipper.h" 


namespace avs { namespace vframe {



//////////////////////////////////////////////////////////////////////////////////////////////
// InterleavedBase
//
//
//
class InterleavedBase : public Base
{
  
  BufferWindow main_;


public:  //structors

  //normal constructor
  InterleavedBase(ColorSpace& space, Dimension const& dim, FrameType type, PEnvironment const& env);

  //conversion constructor
  InterleavedBase(ColorSpace& space, Base const& other);

  //generated copy constructor and destructor are fine


public:  //fetch environment method

  virtual PEnvironment const& GetEnvironment() const { return main_.GetEnvironment(); }


public:  //plane access

  virtual BufferWindow& operator[](Plane plane);
  virtual const BufferWindow& operator[](Plane plane) const;

  BufferWindow& GetMain() { ClearStaticProperties(); return main_; }
  BufferWindow const& GetMain() const { return main_; }
  BufferWindow const& GetConstMain() const { return main_; }


public:  //toolbox methods

  virtual void ChangeSize(Vecteur const& topLeft, Vecteur const& bottomRight);
  virtual void Copy(VideoFrame const& other, Vecteur const& coords);

  virtual void FlipVertical() { GetMain() = bw::VerticalFlipper()(GetConstMain()); }

  virtual void TurnLeft();
  virtual void TurnRight();

  virtual void Blend(VideoFrame const& other, float factor);


private:  //implementation helpers

  virtual BufferWindow DoTurnLeft() = 0;
  virtual BufferWindow DoTurnRight() = 0;

  virtual void DoBlend(BufferWindow const& other, float factor) = 0;

};//InterleavedBase



//////////////////////////////////////////////////////////////////////////////////////////////
// interleaved<int bpp, int bps>
//
// VideoFrame factorisation superclass for interleaved colorspaces (YUY2, RGB24, RGB32, RGB45)
// where all the data is in only one frame buffer
//
template <int bpp, int bps> class interleaved : public InterleavedBase
{

public:  //structors

  //normal constructor
  interleaved(ColorSpace& space, Dimension const& dim, FrameType type, PEnvironment const& env)
    : InterleavedBase( space, dim, type, env ) { }

  //conversion constructor
  interleaved(ColorSpace& space, Base const& other)
    : InterleavedBase( space, other ) { }

  //generated copy constructor and destructor are fine


public:  //toobox methods

  virtual void FlipHorizontal() { GetMain() = bw::HorizontalFlipper<bpp>()(GetConstMain()); }


private:  //implementation helpers

  virtual BufferWindow DoTurnLeft() { return bw::LeftTurner<bpp>()(GetConstMain()); }
  virtual BufferWindow DoTurnRight() { return bw::RightTurner<bpp>()(GetConstMain()); }

  virtual void DoBlend(BufferWindow const& other, float factor) { return bw::Blender<bps>(factor)(GetMain(), other); }

};//interleaved<int bpp, int bps>



} } //namespace avs::vframe

#endif //__AVS_VIDEOFRAME_INTERLEAVED_H__
