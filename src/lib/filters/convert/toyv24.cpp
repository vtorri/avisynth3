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
#include "toyv24.h"
#include "toyv24/fromrgb32.h"
#include "../../core/videoinfo.h"
#include "../../core/colorspace.h"
#include "../../core/colorspace/get.h"
#include "../../core/exception/colorspace/unsupported.h"


namespace avs { namespace filters { namespace convert {



ToYV24::ToYV24(PClip const& child)
  : Convert( child, colorspace::Get::YV24() ) { }


PClip ToYV24::Create(PClip const& child)
{
  PColorSpace space = child->GetVideoInfo()->GetColorSpace();

  switch( space->id() )
  {
  case ColorSpace::I_EXTERNAL: return FromExternal( child, colorspace::Get::YV24() );
  case ColorSpace::I_RGB32:    return toyv24::FromRGB32::Create(child);

  default: throw exception::colorspace::Unsupported(space);
  }
}



} } } //namespace avs::filters::convert
