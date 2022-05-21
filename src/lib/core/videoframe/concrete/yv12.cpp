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
#include "yv12.h"
#include "../../colorspace/get.h"


namespace avs { namespace videoframe { namespace concrete {



YV12::YV12(Dimension const& dim, FrameType type, PEnvironment env)
  : planar::YUV( colorspace::Get::YV12(), dim, type, env ) { }


YV12::YV12(Dimension const& dim, FrameType type, BufferWindow const& y, BufferWindow const& u, BufferWindow const& v)
  : planar::YUV( colorspace::Get::YV12(), dim, type, y, u, v ) { }



CPVideoFrame YV12::clone() const
{
  return CPVideoFrame( static_cast<VideoFrame *>(new YV12(*this)) );
}


PColorSpace YV12::GetColorSpace() const
{
  return colorspace::Get::YV12();
}



} } } //namespace avs::videoframe::concrete
