// Avisynth v3.0 alpha.  Copyright 2005 David Pierre - Ben Rudiak-Gould et al.
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


#include "interleaved.h"
#include "../../ownedblock.h"
#include "../../bufferwindow.h"
#include "../../exception/nosuchplane.h"
#include "../../exception/colorspace/invalidheight.h"


namespace avs { namespace colorspace { namespace concrete {



void Interleaved::Check(long /*x*/, long y, bool interlaced) const
{
  if ( interlaced && (y & 1) )
    throw exception::colorspace::InvalidHeight(shared_from_this(), y, 2, true);
}

void Interleaved::ToPlane(long& x, long& /*y*/, char plane) const
{
  if ( plane != '~' )
    throw exception::NoSuchPlane(shared_from_this(), plane);
  x *= bytesPerPixel_;
}


PVideoFrame Interleaved::CreateFrame(PEnvironment const& env, Dimension const& dim, FrameType type) const
{
  return CreateFrame(dim, type, BufferWindow(ToPlaneDim(dim, '~'), env));
}



} } } //namespace avs::colorspace::concrete
