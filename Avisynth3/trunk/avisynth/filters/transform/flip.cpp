// Avisynth v3.0 alpha.  Copyright 2003 Ben Rudiak-Gould et al.
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
#include "flip.h"
#include "../../videoframe.h"


namespace avs { namespace filters {


CPVideoFrame FlipVertical::ProcessFrame(CPVideoFrame source) const
{
  PVideoFrame result = source;
  result->FlipVertical();
  return result;
}


PClip FlipVertical::Simplify(PClip self) const
{
  if (boost::shared_ptr<const FlipVertical> fv = dynamic_pointer_cast<const FlipVertical>( GetChild() ))
    return fv->GetChild();
  return self;
}


CPVideoFrame FlipHorizontal::ProcessFrame(CPVideoFrame source) const
{
  PVideoFrame result = source;
  result->FlipHorizontal();
  return result;
}


PClip FlipHorizontal::Simplify(PClip self) const
{
  if (boost::shared_ptr<const FlipHorizontal> fh = dynamic_pointer_cast<const FlipHorizontal>( GetChild() ))
    return fh->GetChild();
  return self;
}


}; }; //namespace avs::filters