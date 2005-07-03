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


#ifdef _WIN32

//avisynth includes
#include "vfwyv12.h"
#include "../../../../core/colorspace.h"
#include "../../../../core/ownedblock.h"
#include "../../../../core/bufferwindow.h"
#include "../../../../core/videoframe/concrete/yv12.h"


namespace avs { namespace filters { namespace source { namespace video { namespace importer {



PColorSpace VFWYV12::GetColorSpace() const
{
  return ColorSpace::yv12();
}


CPVideoFrame YV12::CreateFrame(Dimension const& dim, owned_block<1> const& block) const
{
  Dimension dimUV = dim.Divide<2, 2>();

  //split into independant blocks each containing a plane
  owned_block<1> blockY = block;
  owned_block<1> blockU = blockY.Split( dim.BMPSize<4>() );
  owned_block<1> blockV = blockU.Split( dimUV.BMPSize<2>() );

  //promote the blocks into frame buffers with the proper alignment
  buffer_window<4> y( dim, blockY, 0 );
  buffer_window<2> u( dimUV, blockU, 0 );
  buffer_window<2> v( dimUV, blockV, 0 );

  return CPVideoFrame( static_cast<VideoFrame *>(new vframe::concrete::YV12(dim, UNKNOWN, y, u, v)) );
}


namespace {

Importer const * CreateYV12Importer() { return new VFWYV12Importer(); }

}


VFWYV12::ValueCache const VFWYV12::yv12(&CreateYV12Importer);



} } } } } //namespace avs::filters::source::video::importer

#endif //_WIN32
