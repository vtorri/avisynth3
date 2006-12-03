// Avisynth v3.0 alpha.  Copyright 2004-2006 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_COLORSPACE_PLANAR_H__
#define __AVS_COLORSPACE_PLANAR_H__

//avisynth include
#include "../colorspace.h"

//stlport include
#include <cassert>
#include <vector>


namespace avs { namespace cspace {



////////////////////////////////////////////////////////////////////////////////////////////////////////
//  cspace::Planar
//
//  ColorSpace sub interface for planar color spaces
//
class Planar : public virtual ColorSpace
{

public:  //Planar interface

  virtual PVideoFrame CreateFrame(Dimension const& dim, FrameType type, std::vector<BufferWindow> const& planes) const = 0;

};



namespace planar {



///////////////////////////////////////////////////////////////////////////////////////////////////
//  cspace::planar::YUV
//
//  Planar subinterface for planar YUV color spaces
//
class YUV : public Planar
{

public:  //Planar interface

  virtual PVideoFrame CreateFrame(Dimension const& dim, FrameType type, std::vector<BufferWindow> const& planes) const
  {
    assert( planes.size() == 3 );
    return CreateFrame(dim, type, planes[0], planes[1], planes[2]);
  }


public:  //planar::YUV interface

  //create a frame using the given buffers
  virtual PVideoFrame CreateFrame(Dimension const& dim, FrameType type, BufferWindow const& y , BufferWindow const& u, BufferWindow const& v) const = 0;

};



///////////////////////////////////////////////////////////////////////////////////////////////////
//  cspace::planar::RGB
//
//  Planar subinterface for planar RGB color spaces
//
class RGB : public Planar
{

public:  //Planar interface

  virtual PVideoFrame CreateFrame(Dimension const& dim, FrameType type, std::vector<BufferWindow> const& planes) const
  {
    assert( planes.size() == 3 );
    return CreateFrame(dim, type, planes[0], planes[1], planes[2]);
  }


public:  //planar::RGB interface

  //create a frame using the given buffers
  virtual PVideoFrame CreateFrame(Dimension const& dim, FrameType type, BufferWindow const& r , BufferWindow const& g, BufferWindow const& b) const = 0;

};


} } } //namespace avs::cspace::planar

#endif //__AVS_COLORSPACE_PLANAR_H__
