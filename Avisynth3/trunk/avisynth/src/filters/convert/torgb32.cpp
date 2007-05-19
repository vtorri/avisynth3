// Avisynth v3.0 alpha.  Copyright 2003-2007 David Pierre - Ben Rudiak-Gould et al.
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
#include "torgb32.h"
#include "torgb32/fromyv12.h"
#include "torgb32/fromyuy2.h"
#include "../../core/videoinfo.h"
#include "../../core/colorspace.h"
#include "../../core/colorspace/get.h"
#include "../../core/exception/colorspace/unsupported.h"


namespace avs { namespace filters { namespace convert {



ToRGB32::ToRGB32(PClip const& child)
  : Convert( child, colorspace::Get::RGB32() ) { }


PClip ToRGB32::Create(PClip const& child)
{
  PColorSpace space = child->GetVideoInfo()->GetColorSpace();

  switch( space->id() )
  {
  case ColorSpace::I_EXTERNAL: return FromExternal( child, colorspace::Get::RGB32() );
  case ColorSpace::I_YV12:     return torgb32::FromYV12::Create(child);
  case ColorSpace::I_YUY2:     return torgb32::FromYUY2::Create(child);

  default: throw exception::colorspace::Unsupported(space);
  }
}



} } } //namespace avs::filters::convert
