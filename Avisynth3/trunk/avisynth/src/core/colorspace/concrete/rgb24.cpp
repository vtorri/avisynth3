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
#include "rgb24.h"
#include "../../videoframe/concrete/rgb24.h"


namespace avs { namespace colorspace { namespace concrete {



PVideoFrame RGB24::CreateFrame(PEnvironment const& env, Dimension const& dim, FrameType type) const
{
  return CPVideoFrame( static_cast<VideoFrame *>(new videoframe::concrete::RGB24(dim, type, env)) );
}

PVideoFrame RGB24::CreateFrame(Dimension const& dim, FrameType type, BufferWindow const& main) const
{
  return CPVideoFrame( static_cast<VideoFrame *>(new videoframe::concrete::RGB24(dim, type, main)) );
}



} } } //namespace avs::colorspace::concrete
