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


#ifndef __AVS_CS_INTERLEAVED_H__
#define __AVS_CS_INTERLEAVED_H__

//avisynth includes
#include "../colorspace.h"
#include "../exception/nosuchplane.h"


namespace avs { namespace cspace {



////////////////////////////////////////////////////////////////////////////////////////
//  interleaved<bpp, bps, VFrame>
//
//
//
template <int bpp_, int bps_, typename VFrame> 
class interleaved : public Interleaved
{

public:  //declarations and typedefs

  enum { bpp = bpp_, bps = bps_ };

  typedef interleaved<bpp, bps, VFrame> InterleavedType;


public:  //ColorSpace interface

  virtual int GetBitmapSize(Dimension const& dim) const
  {
    return RoundUp<4>( dim.GetWidth() * bpp ) * dim.GetHeight();
  }

  virtual bool HasPlane(Plane plane) const { return plane == NOT_PLANAR; }

  virtual void ToPlane(int& x, int& /*y*/, Plane plane) const
  {
    if ( plane != NOT_PLANAR )
      throw exception::NoSuchPlane(*this, plane);
    x *= bpp;
  }

  virtual PVideoFrame CreateFrame(PEnvironment const& env, Dimension const& dim, FrameType type) const
  {
    return CPVideoFrame( static_cast<VideoFrame *>(new VFrame(dim, type, env)) );
  }


public:  //Interleaved interface

  virtual PVideoFrame CreateFrame(Dimension const& dim, FrameType type, BufferWindow const& main) const
  {
    return CPVideoFrame( static_cast<VideoFrame *>(new VFrame(dim, type, main)) );
  }

};




} } //namespace avs::cspace

#endif //__AVS_CS_INTERLEAVED_H__
