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
#include "resize.h"
#include "../../core/videoinfo.h"
#include "../../core/videoframe.h"


namespace avs { namespace filters {



Resize::Resize(PClip const& child, resize::PFilter const& filter, Dimension const& dim, resize::SubRange const& subrange)
  : clip::onechild::Concrete( child )
  , clip::framemaker::Concrete( child->GetEnvironment() )
  , filter_( filter )
  , subrange_( subrange )
{
  PVideoInfo vi = child->GetVideoInfo();  //start from child VideoInfo
  
  vi->SetDimension(dim);                  //update Dimension

  vi_ = vi;                               //save it
}


CPVideoFrame Resize::MakeFrame(PVideoFrame const& source) const
{
  PVideoFrame target = GetEnvironment()->CreateFrame(*GetVideoInfo(), source->GetType());

  ResizeFrame(*source, *target);

  return target;
}



} } //namespace avs::filters
