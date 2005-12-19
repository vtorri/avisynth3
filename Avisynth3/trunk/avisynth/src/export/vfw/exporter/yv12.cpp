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
#include "../../../core/blitter.h"
#include "../../../core/videoframe.h"
#include "../../../core/utility/round.h"
#include "../../../core/geometry/dimension.h"


namespace avs { namespace export_ { namespace vfw { namespace exporter {



long YV12::GetBitmapSize(Dimension const& dim) const
{
  return 3 * dim.GetHeight() * utility::RoundUp<4>(dim.GetWidth()) / 2;
}


void YV12::ExportFrame(VideoFrame const& frame, BYTE * ptr) const
{
  CWindowPtr Y = frame.ReadFrom('Y');   
  CWindowPtr U = frame.ReadFrom('U'); 
  CWindowPtr V = frame.ReadFrom('V'); 

  Blitter const& blitter = Blitter::Get();

  long dstPitch = utility::RoundUp<4>(Y.width);

  ptr = blitter.Blit(Y, ptr, dstPitch);
  ptr = blitter.Blit(U, ptr, dstPitch >> 1);
        blitter.Blit(V, ptr, dstPitch >> 1);
}



} } } } //namespace avs::export_::vfw::exporter

