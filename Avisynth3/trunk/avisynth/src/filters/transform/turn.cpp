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


//avisynth includes
#include "turn.h"
#include "flip.h"
#include "symetry.h"
#include "../../core/videoinfo.h"
#include "../../core/dimension.h"
#include "../../core/videoframe.h"
#include "../../core/cow_shared_ptr.h"


namespace avs { namespace filters {



CPVideoInfo Turn::GetVideoInfo() const
{
  PVideoInfo vi = GetChildVideoInfo();
  vi->SetDimension( vi->GetDimension().Turn<1>() );
  return vi;
}



PClip Turn::Refactor(Flip const& parent) const
{
  return clone( parent.OtherFlip(GetChild()) );
}



CPVideoFrame Turn::Left::MakeFrame(CPVideoFrame const& source) const
{
  PVideoFrame result = source;
//  result->TurnLeft();
  return result;
}


PClip Turn::Left::MirrorTurn(PClip const& child) const
{
  return Right::Create(child);
}


PClip Turn::Left::TurnLeft() const
{
  return Symetry::Create(GetChild());
}



CPVideoFrame Turn::Right::MakeFrame(CPVideoFrame const& source) const
{
  PVideoFrame result = source;
//  result->TurnRight();
  return result;
}


PClip Turn::Right::MirrorTurn(PClip const& child) const
{
  return Left::Create(child);
}


PClip Turn::Right::TurnRight() const
{
  return Symetry::Create(GetChild());
}


} } //namespace avs::filters
