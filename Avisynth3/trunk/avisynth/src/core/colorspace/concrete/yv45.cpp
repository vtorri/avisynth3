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


//avisynth includes
#include "yv45.h"
#include "../../exception/nosuchplane.h"
#include "../../videoframe/concrete/yv45.h"
#include "../../exception/colorspace/unsupported.h"
#include "../../exception/colorspace/invalidheight.h"


namespace avs { namespace cspace { namespace concrete {



unsigned long YV45::GetFourCC() const
{
  throw exception::cspace::Unsupported( shared_from_this() );
}


long YV45::GetBitmapSize(Dimension const& dim) const
{
  return 3 * RoundUp<4>(dim.GetWidth() * 2) * dim.GetHeight();
}


bool YV45::HasProperty(Property prop) const
{
  return prop == P_PLANAR || prop == P_YUV || prop == P_DEPTH15;
}



void YV45::Check(long /*x*/, long y, bool interlaced) const
{
  if ( interlaced && (y & 1) )
    throw exception::cspace::InvalidHeight(shared_from_this(), y, 2, true);
}


void YV45::ToPlane(long& x, long& /*y*/, Plane plane) const
{
  if ( ! HasPlane(plane) )
    throw exception::NoSuchPlane(shared_from_this(), plane);

  x <<= 1;
}



PVideoFrame YV45::CreateFrame(PEnvironment const& env, Dimension const& dim, FrameType type) const
{
  return CPVideoFrame( static_cast<VideoFrame *>(new vframe::concrete::YV45(dim, type, env)) );
}


PVideoFrame YV45::CreateFrame(Dimension const& dim, FrameType type, BufferWindow const& y , BufferWindow const& u, BufferWindow const& v) const
{
  return CPVideoFrame( static_cast<VideoFrame *>(new vframe::concrete::YV45(dim, type, y, u, v)) );
}



} } } //namespace avs::cspace::concrete
