// Avisynth v3.0 alpha.  Copyright 2004 David Pierre - Ben Rudiak-Gould et al.
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
#include "../avisource.h"
#include "framedecompressor.h"
#include "../../../core/videoinfo.h"
#include "../../../core/colorspace.h"
#include "../../../core/exception/fatal.h"


namespace avs { namespace filters { namespace avisource {



CPVideoFrame FrameDecompressor::operator()(int n, bool preroll)
{
  std::pair<OwnedBlock, long> read = src_.ReadVideo(n);

  if ( read.second == 0 )                 //size 0 means dropped frame
    return CPVideoFrame();

  OwnedBlock block = operator()(src_.NearestKeyFrame(n) == n, preroll, read.first, read.second);

  CPVideoInfo vi = src_.GetVideoInfo();
  ColorSpace& space = vi->GetColorSpace();
  Dimension dim = vi->GetDimension();

  switch( space.id() )
  {
  case ColorSpace::I_RGB24:
  case ColorSpace::I_RGB32:
  case ColorSpace::I_YUY2:
    {
      //create a 4-bytes aligned frame buffer of the expected size by promoting the block
      //since we respected guards into the block it shouldn't blit
      buffer_window<4> main( space.ToPlaneDim(dim, NOT_PLANAR), block, Guard );
    
      //uses space (casted to Interleaved) to create a frame from it
      //if the size was favorable the conversion from buffer_window<4>
      //to buffer_window<block::Align> (ie BufferWindow) may not blit
      return static_cast<cspace::Interleaved&>(space).CreateFrame(dim, UNKNOWN, main);
    }
  
  case ColorSpace::I_YV12:
    {
      //like above, promote the block into frame buffers with the proper alignment
      //they are overlapping, thus breaking the guard requirement
      //but it's not a problem because since they are sharing the same memory block
      //they will blit on write, restoring the guards at this time (supposing align stuff didn't already blit)
      buffer_window<4> y( space.ToPlaneDim(dim, PLANAR_Y), block, Guard );
      buffer_window<2> u( space.ToPlaneDim(dim, PLANAR_U), block, Guard + y.size() );
      buffer_window<2> v( space.ToPlaneDim(dim, PLANAR_V), block, Guard + y.size() + u.size() );

      return static_cast<cspace::PlanarYUV&>(space).CreateFrame(dim, UNKNOWN, y, u, v);
    }

  default: throw exception::Fatal();
  }

}


} } } //namespace avs::filters::avisource
