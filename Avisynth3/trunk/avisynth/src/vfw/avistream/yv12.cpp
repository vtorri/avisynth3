// Avisynth v3.0 alpha.  Copyright 2003 David Pierre - Ben Rudiak-Gould et al.
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


//avisynth include
#include "yv12.h"
#include "../../core/blitter.h"
#include "../../core/dimension.h"
#include "../../core/videoframe.h"


namespace avs { namespace vfw { namespace avistream {


int YV12::GetBMPSize(Dimension const& dim)
{
  return 3 * dim.GetHeight() * RoundUp<4>(dim.GetWidth()) / 2;
}


void YV12::ReadFrame(VideoFrame const& frame, BYTE * ptr)
{
  CWindowPtr Y = frame.ReadFrom(PLANAR_Y);  
  CWindowPtr U = frame.ReadFrom(PLANAR_U);  
  CWindowPtr V = frame.ReadFrom(PLANAR_V);  

  int pitch = RoundUp<4>(Y.width);

  Blitter const& blit = Blitter::Get();

  blit(Y, ptr, pitch);                                                //blits Y
  blit(U, ptr + pitch * Y.height, pitch / 2);                         //blits U
  blit(V, ptr + pitch * Y.height + pitch / 2 * U.height, pitch / 2);  //blits V
}



} } } //namespace avs::vfw::avistream
