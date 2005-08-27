// Avisynth v3.0 alpha.  Copyright 2003-2005 David Pierre - Ben Rudiak-Gould et al.
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
  , clip::framemaker::Concrete( first->GetEnvironment() )
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

 
BYTE * Stack::GetAudio(BYTE * /*buf*/, long long /*start*/, long /*count*/) const
{
  throw exception::NoAudio();
}


CPVideoFrame Stack::operator()(long n) const
{
  CPVideoFrame left = GetLeftFrame(n);
  CPVideoFrame right = GetRightFrame(n);

  PEnvironment const& env = GetEnvironment();
  Blitter const& blitter = env->GetBlitter();
  PColorSpace space = left->GetColorSpace();

  PVideoFrame result = env->CreateFrame(*vi_, left->GetType());
  Vecteur shift = GetShiftVecteur();

  std::string planeList = vi_->GetColorSpace()->GetPlaneList();
  for ( std::string::iterator it = planeList.begin(); it != planeList.end(); ++it )
  {
    WindowPtr dst = result->WriteTo(*it);

    blitter.Blit( left->ReadFrom(*it), dst.ptr, dst.pitch );
    blitter.Blit( right->ReadFrom(*it), dst.at(space->ToPlaneVect(shift, *it)), dst.pitch );
  }

  return result;
}



} } //namespace avs::filters
