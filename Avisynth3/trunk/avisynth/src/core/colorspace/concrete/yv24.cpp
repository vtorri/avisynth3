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
#include "yv24.h"
#include "../../exception/nosuchplane.h"
#include "../../videoframe/concrete/yv24.h"
#include "../../exception/colorspace/unsupported.h"
#include "../../exception/colorspace/invalidheight.h"


namespace avs { namespace cspace { namespace concrete {



unsigned long YV24::GetFourCC() const
{
  throw exception::cspace::Unsupported( shared_from_this() );
}


long YV24::GetBitmapSize(Dimension const& dim) const
{
  return 3 * utility::RoundUp<4>(dim.GetWidth()) * dim.GetHeight();
}


bool YV24::HasProperty(Property prop) const
{
  return prop == P_PLANAR || prop == P_YUV || prop == P_DEPTH8;
}


void YV24::Check(long x, long y, bool interlaced) const
{
  if ( interlaced && (y & 1) )                  //if interlaced y must be even
    throw exception::cspace::InvalidHeight(shared_from_this(), y, 2, true);
}


void YV24::ToPlane(long& x, long& y, Plane plane) const
{
  if ( ! HasPlane(plane) )
    throw exception::NoSuchPlane(shared_from_this(), plane);    
}



PVideoFrame YV24::CreateFrame(PEnvironment const& env, Dimension const& dim, FrameType type) const
{
  return CPVideoFrame( static_cast<VideoFrame *>(new vframe::concrete::YV24(dim, type, env)) );
}


PVideoFrame YV24::CreateFrame(Dimension const& dim, FrameType type, BufferWindow const& y , BufferWindow const& u, BufferWindow const& v) const
{
  return CPVideoFrame( static_cast<VideoFrame *>(new vframe::concrete::YV24(dim, type, y, u, v)) );
}

} } } //namespace avs::cspace::concrete
