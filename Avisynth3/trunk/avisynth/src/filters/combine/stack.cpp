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
#include "stack.h"
#include "../../core/blitter.h"
#include "../../core/videoinfo.h"


namespace avs { namespace filters { 



Stack::Stack(PClip const& first, PClip const& second)
  : clip::twochilds::Concrete( first, second )
  , clip::caching::Concrete( first->GetEnvironment() )
{
  CPVideoInfo firstVi = first->GetVideoInfo();
  CPVideoInfo secondVi = second->GetVideoInfo();

  firstVi->CheckColorSpaceMatch(*secondVi);
  firstVi->CheckFPSMatch(*secondVi);
  firstVi->CheckFrameFlagMatch(*secondVi);
}

  
void Stack::BlitPlane(WindowPtr const& dst, CWindowPtr const& firstSrc, CWindowPtr const& secondSrc, int offset) const
{
  Blitter const& blit = GetEnvironment()->GetBlitter();

  blit(firstSrc, dst.ptr, dst.pitch);
  blit(secondSrc, dst.ptr + offset, dst.pitch);
}


} } //namespace avs::filters
