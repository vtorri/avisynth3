// Avisynth v3.0 alpha.  Copyright 2003-2005 David Pierre - Ben Rudiak-Gould et al.
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


//avisynth includes
#include "memcopy.h"
#include "../window_ptr.h"
#include "../geometry/dimension.h"



namespace avs {


BYTE * Blitter::Blit(CWindowPtr const& src, BYTE * ptr, int pitch) const
{
  BYTE const * srcp = src.ptr;

  Blit(srcp, src.pitch, ptr, pitch, Dimension(src.width, src.height));

  return ptr;
}


BYTE const * Blitter::Blit(BYTE const * src, int pitch, WindowPtr const& dst) const
{
  BYTE * dstp = dst.ptr;

  Blit(src, pitch, dstp, dst.pitch, Dimension(dst.width, dst.height));

  return src;
}


void Blitter::Blit(CWindowPtr const& src, WindowPtr const& dst, Dimension const& dim) const
{
  BYTE const * srcp = src.ptr;
  BYTE * dstp = dst.ptr;

  Blit(srcp, src.pitch, dstp, dst.pitch, dim);
}



Blitter const& Blitter::Get()
{
  return blitter::MemCopy::instance;
}



} //namespace avs
