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
#include "turn.h"
#include "../../dimension.h"
#include "../../videoframe.h"


namespace avs { namespace filters {



Turn::Turn(PClip child)
  : Pipeline( child )
  , vi_( child->GetVideoInfo() )
{
  Dimension dim = vi_.GetDimension();

  int height = dim.GetHeight();

  dim.SetHeight( dim.GetWidth() );
  dim.SetWidth( height );

  vi_.SetDimension( dim );
}



CPVideoFrame Turn::Left::ProcessFrame(CPVideoFrame source) const
{
  PVideoFrame result = source;
  result->TurnLeft();
  return result;
}


PClip Turn::Left::Simplify(PClip self) const
{
  if (boost::shared_ptr<Right const> tr = dynamic_pointer_cast<Right const>( GetChild() ))
    return tr->GetChild();
  return self;
}


CPVideoFrame Turn::Right::ProcessFrame(CPVideoFrame source) const
{
  PVideoFrame result = source;
  result->TurnRight();
  return result;
}


PClip Turn::Right::Simplify(PClip self) const
{
  if (boost::shared_ptr<Left const> tl = dynamic_pointer_cast<Left const>( GetChild() ))
    return tl->GetChild();
  return self;
}


}; }; //namespace avs::filters