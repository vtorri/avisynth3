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


#ifndef __AVS_VIDEOFRAME_PLANAR_YUV_H__
#define __AVS_VIDEOFRAME_PLANAR_YUV_H__

//avisynth includes
#include "base.h"
#include "../colorspace.h"
#include "../ownedblock.h"
#include "../bufferwindow.h"
#include "../exception/nosuchplane.h"

//assert include
#include "assert.h"


namespace avs { namespace vframe {



//////////////////////////////////////////////////////////////////////////////////////////
//  PlanarYUV
//
//
//
class PlanarYUV : public Base
{

  BufferWindow y_, u_, v_;


public:  //structors

  //normal constructor
  PlanarYUV(PColorSpace const& space, Dimension const& dim, FrameType type, PEnvironment const& env)
    : Base( space, dim, type )
    , y_( space->ToPlaneDim(dim, PLANAR_Y), env )
    , u_( space->ToPlaneDim(dim, PLANAR_U), env )
    , v_( space->ToPlaneDim(dim, PLANAR_V), env ) { }

  //constructs using given buffers
  PlanarYUV(PColorSpace const& space, Dimension const& dim, FrameType type, BufferWindow const& y, BufferWindow const& u, BufferWindow const& v)
    : Base( space, dim, type )
    , y_( y )
    , u_( u )
    , v_( v )
  {
    assert( y.GetDimension() == space->ToPlaneDim(dim, PLANAR_Y) );
    assert( u.GetDimension() == space->ToPlaneDim(dim, PLANAR_U) );
    assert( v.GetDimension() == space->ToPlaneDim(dim, PLANAR_V) );
    assert( y.GetEnvironment() == u.GetEnvironment() && y.GetEnvironment() == v.GetEnvironment() );
  }

  //generated copy constructor and destructor are fine


public:  //fetch environment method

  virtual PEnvironment const& GetEnvironment() const { return y_.GetEnvironment(); }


public:  //plane access

  virtual BufferWindow& operator[](Plane plane)
  {
    switch(plane)
    {
    case PLANAR_Y: return GetY();
    case PLANAR_U: return GetU();
    case PLANAR_V: return GetV();
    
    default: throw exception::NoSuchPlane(GetColorSpace(), plane);
    }   
  }

  virtual BufferWindow const& operator[](Plane plane) const
  {
    switch(plane)
    {
    case PLANAR_Y: return GetConstY();
    case PLANAR_U: return GetConstU();
    case PLANAR_V: return GetConstV();

    default: throw exception::NoSuchPlane(GetColorSpace(), plane);
    }
  }

  BufferWindow& GetY() { ClearStaticProperties(); return y_; }
  BufferWindow& GetU() { ClearStaticProperties(); return u_; }
  BufferWindow& GetV() { ClearStaticProperties(); return v_; }

  BufferWindow const& GetConstY() const { return y_; }
  BufferWindow const& GetConstU() const { return u_; }
  BufferWindow const& GetConstV() const { return v_; }

};



//////////////////////////////////////////////////////////////////////////////////////////
//  YV12
//
//  YV12 specific VideoFrame subclass
//
class YV12 : public PlanarYUV
{

public:  //structors

  //normal constructor
  YV12(Dimension const& dim, FrameType type, PEnvironment env)
    : PlanarYUV( ColorSpace::yv12(), dim, type, env ) { }

  //constructs using given buffers
  YV12(Dimension const& dim, FrameType type, BufferWindow const& y, BufferWindow const& u, BufferWindow const& v)
    : PlanarYUV( ColorSpace::yv12(), dim, type, y, u, v ) { }

  //generated copy constructor and destructor are fine


public:  //clone method

  virtual CPVideoFrame clone() const { return CPVideoFrame( new YV12(*this) ); }


public:  //general frame info

  virtual PColorSpace GetColorSpace() const { return ColorSpace::yv12(); }


};



//////////////////////////////////////////////////////////////////////////////////////////
//  YV24
//
//  YV24 specific VideoFrame subclass
//
class YV24 : public PlanarYUV
{

public:  //constructors

  //normal constructor
  YV24(Dimension const& dim, FrameType type, PEnvironment const& env)
    : PlanarYUV( ColorSpace::yv24(), dim, type, env ) { }

  //constructs using given buffers
  YV24(Dimension const& dim, FrameType type, BufferWindow const& y, BufferWindow const& u, BufferWindow const& v)
    : PlanarYUV( ColorSpace::yv24(), dim, type, y, u, v ) { }

  //generated copy constructor and destructor are fine


public:  //clone method

  virtual CPVideoFrame clone() const { return CPVideoFrame( (VideoFrame *)new YV24(*this) ); }


public:  //general frame info

  virtual PColorSpace GetColorSpace() const { return ColorSpace::yv24(); }

};



//////////////////////////////////////////////////////////////////////////////////////////
//  YV45
//
//  YV45 specific VideoFrame subclass
//
class YV45 : public PlanarYUV
{

public:  //constructors

  //normal constructor
  YV45(Dimension const& dim, FrameType type, PEnvironment const& env)
    : PlanarYUV( ColorSpace::yv45(), dim, type, env ) { }

  //constructs using given buffers
  YV45(Dimension const& dim, FrameType type, BufferWindow const& y, BufferWindow const& u, BufferWindow const& v)
    : PlanarYUV( ColorSpace::yv45(), dim, type, y, u, v ) { }

  //generated copy constructor and destructor are fine


public:  //clone method

  virtual CPVideoFrame clone() const { return CPVideoFrame( (VideoFrame *)new YV45(*this) ); }


public:  //general frame info

  virtual PColorSpace GetColorSpace() const { return ColorSpace::yv45(); }

};



} } //namespace avs::vframe

#endif //__AVS_VIDEOFRAME_PLANAR_YUV_H__
