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


#ifndef __AVS_BLITTER_H__
#define __AVS_BLITTER_H__

//avisynth includes
#include "window_ptr.h"
#include "dimension.h"

//boost include
#include <boost/utility.hpp>


namespace avs {


//declaration
class BufferWindow;



class Blitter : private boost::noncopyable
{

public:  

  //others are implemented in terms of this version
  virtual void operator()(BYTE const * srcp, int srcPitch, BYTE * dstp, int dstPitch, Dimension const& dim) const = 0;

  //if src and dst are not of same Dimension, blits common part only
  //potentially make take advantage of BufferWindow alignment
  virtual void operator()(BufferWindow const& src, BufferWindow& dst) const;


  void operator()(BYTE const * srcp, int srcPitch, BYTE * dstp, int dstPitch, int width, int height) const
  {
    return operator()(srcp, srcPitch, dstp, dstPitch, Dimension(width, height));
  }

  void operator()(CWindowPtr const& src, BYTE * ptr, int pitch) const
  {
    return operator()(src.ptr, src.pitch, ptr, pitch, Dimension(src.width, src.height));
  }

  void operator()(CWindowPtr const& src, WindowPtr const& dst, Dimension const& dim) const
  {
    return operator()(src.ptr, src.pitch, dst.ptr, dst.pitch, dim);
  }

public:

  static Blitter& Get();

};



} //namespace avs

#endif //__AVS_BLITTER_H__