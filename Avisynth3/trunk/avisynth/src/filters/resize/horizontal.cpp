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
#include "horizontal.h"
#include "horizontal/rgb24.h"
#include "horizontal/rgb32.h"
#include "horizontal/yuy2.h"
#include "../../core/videoinfo.h"
#include "../../core/geometry/dimension.h"
#include "../../core/exception/colorspace/unsupported.h"


//stl include
#include "assert.h"


namespace avs { namespace filters { namespace resize {



Horizontal::Horizontal(PClip const& child, PFilter const& filter, int width, SubRange const& subrange)
  : Resize( child, filter, Dimension(width, child->GetVideoInfo()->GetHeight()), subrange )
{ 
  assert( subrange.whole() == child->GetVideoInfo()->GetWidth() );
}


PClip Horizontal::clone(PClip const& child) const
{
  return Create(child, GetFilter(), GetVideoInfo()->GetWidth(), SubRange(GetSubRange(), child->GetVideoInfo()->GetWidth()) );
}



PClip Horizontal::Create(PClip const& child, PFilter const& filter, int width, SubRange const& subrange)
{
  PColorSpace const& space = child->GetVideoInfo()->GetColorSpace();

  switch( space->id() )
  {
  case ColorSpace::I_RGB24: return horizontal::RGB24::Create(child, filter, width, subrange);
  case ColorSpace::I_RGB32: return horizontal::RGB32::Create(child, filter, width, subrange);
  case ColorSpace::I_YUY2:  return horizontal::YUY2::Create(child, filter, width, subrange);

  default: throw exception::cspace::Unsupported( space );
  }
}



} } } //namespace avs::filters::resize
