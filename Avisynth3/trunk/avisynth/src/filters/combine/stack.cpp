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
#include "../../core/colorspace.h"
#include "../../core/videoframe.h"
#include "../../core/exception/noaudio.h"


namespace avs { namespace filters { 



Stack::Stack(PClip const& first, PClip const& second, Dimension const& expand)
  : clip::twochilds::Concrete( first, second )
  , clip::caching::Concrete( first->GetEnvironment() )
{
  PVideoInfo vi = first->GetVideoInfo();
  CPVideoInfo secondVi = second->GetVideoInfo();

  vi->CheckColorSpaceMatch(*secondVi);
  vi->CheckFPSMatch(*secondVi);
  vi->CheckFrameFlagMatch(*secondVi);
  vi->CheckFrameCountMatch(*secondVi);

  vi->KillAudio();
  vi->SetDimension(vi->GetDimension() + expand);

  vi_ = vi;
}


BYTE * Stack::GetAudio(BYTE * buffer, long long start, int count) const
{
  throw exception::NoAudio();
}
 

CPVideoFrame Stack::MakeFrame(int n) const
{
  CPVideoFrame left = GetLeftFrame(n);
  CPVideoFrame right = GetRightFrame(n);

  PEnvironment const& env = GetEnvironment();
  Blitter const& blit = env->GetBlitter();
  ColorSpace& space = left->GetColorSpace();

  PVideoFrame result = env->CreateFrame(*vi_, left->GetType());
  Vecteur shift = GetShiftVecteur();

  for ( Plane p = Plane(0); p < int(PlaneCount); p = Plane(p + 1) )
    if ( space.HasPlane(p) )
    {
      WindowPtr dst = result->WriteTo(p);

      blit( left->ReadFrom(p), dst.ptr, dst.pitch );
      blit( right->ReadFrom(p), dst.at(space.ToPlaneVect(shift, p)), dst.pitch );
    }

  return result;
}



} } //namespace avs::filters
