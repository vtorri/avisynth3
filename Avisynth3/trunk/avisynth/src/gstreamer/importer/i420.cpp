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


#ifndef _WIN32

//avisynth includes
#include "i420.h"
#include "../../core/colorspace.h"
#include "../../core/ownedblock.h"
#include "../../core/bufferwindow.h"
#include "../../core/videoframe/concrete/yv12.h"


namespace avs { namespace gstreamer { namespace importer {



PColorSpace I420::GetColorSpace() const
{
  return ColorSpace::yv12();
}


PVideoFrame I420::CreateFrame(Dimension const& dim, OwnedBlock const& block) const
{
  Dimension dimUV = dim.Divide<2, 2>();

  //promote the block into frame buffers with the proper alignment
  //they are overlapping, thus breaking the guard requirement
  //but it's not a problem because since they are sharing the same memory block
  //they will blit on write, restoring the guards at this time (supposing align stuff didn't already blit)
  buffer_window<4> y( dim, block, 0 );
  buffer_window<4> v( dimUV, block, y.size() );
  buffer_window<4> u( dimUV, block, y.size() + v.size() );

  return CPVideoFrame( static_cast<VideoFrame *>(new vframe::concrete::YV12(dim, UNKNOWN, y, v, u)) );
}



} } } //namespace avs::gstreamer::importer

#endif //_WIN32