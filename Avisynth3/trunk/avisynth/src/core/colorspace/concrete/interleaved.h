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


#ifndef __AVS_COLORSPACE_CONCRETE_INTERLEAVED_H__
#define __AVS_COLORSPACE_CONCRETE_INTERLEAVED_H__

//avisynth includes
#include "../interleaved.h"
#include "../../exception/nosuchplane.h"
#include "../../exception/colorspace/invalidheight.h"


namespace avs { namespace cspace { namespace concrete {



////////////////////////////////////////////////////////////////////////////////////////
//  interleaved<bpp, bps, VFrame>
//
//
//
template <long bpp_, long bps_, typename VFrame> 
class AVS_NOVTABLE interleaved : public Interleaved
{

public:  //declarations and typedefs

  enum { bpp = bpp_, bps = bps_ };

  typedef interleaved<bpp, bps, VFrame> InterleavedType;


public:  //ColorSpace interface

  virtual long GetBitsPerPixel() const { return 8 * bpp; }

  virtual long GetBitmapSize(Dimension const& dim) const
  {
    return utility::RoundUp<4>( dim.GetWidth() * bpp ) * dim.GetHeight();
  }

  virtual void Check(long /*x*/, long y, bool interlaced) const
  {
    if ( interlaced && (y & 1) )
      throw exception::cspace::InvalidHeight(shared_from_this(), y, 2, true);
  }

  virtual void ToPlane(long& x, long& /*y*/, Plane plane) const
  {
    if ( plane != NOT_PLANAR )
      throw exception::NoSuchPlane(shared_from_this(), plane);
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




} } } //namespace avs::cspace::concrete

#endif //__AVS_COLORSPACE_CONCRETE_INTERLEAVED_H__
