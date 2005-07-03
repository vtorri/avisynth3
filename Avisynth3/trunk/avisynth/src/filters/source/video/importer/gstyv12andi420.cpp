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
#include "gstyv12andi420.h"
#include "../../../../core/colorspace.h"
#include "../../../../core/ownedblock.h"
#include "../../../../core/bufferwindow.h"
#include "../../../../core/videoframe/concrete/yv12.h"



namespace avs { namespace gstreamer { namespace importer {



PColorSpace GstYV12AndI420::GetColorSpace() const
{
  return ColorSpace::yv12();
}


PVideoFrame GstYV12AndI420::CreateFrame(Dimension const& dim, owned_block<1> const& block) const
{
  Dimension dimUV = dim.Divide<2, 2>();

  //split the block into 3 independant blocks each containing a plane data
  owned_block<1> blockY = block;
  owned_block<1> blockU = blockY.Split( dim.BMPSize<4>() );
  owned_block<1> blockV = blockU.Split( dimUV.BMPSize<4>() );

  if ( swapUV_ )
    swap( blockU, blockV );

  //promote blocks into aligned frame buffers, should not blit  
  buffer_window<4> y( dim, blockY, 0 );
  buffer_window<4> u( dim, blockU, 0 );
  buffer_window<4> v( dim, blockV, 0 );

  //implicitly convert buffer_window<4> into BufferWindow
  //may not blit if frame size is favorable
  return CPVideoFrame( static_cast<VideoFrame *>(new vframe::concrete::YV12(dim, UNKNOWN, y, u, v)) );
}


namespace {

Importer const * CreateYV12Importer() { return new GstYV12AndI420(false); }
Importer const * CreateI420Importer() { return new GstYV12AndI420(true); }

}


GstYV12AndI420::ValueCache const GstYV12AndI420::yv12(CreateYV12Importer);
GstYV12AndI420::ValueCache const GstYV12AndI420::i420(CreateI420Importer);



} } } //namespace avs::gstreamer::importer

#endif //_WIN32
