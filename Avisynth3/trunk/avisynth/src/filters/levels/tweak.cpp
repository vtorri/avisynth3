// Avisynth v3.0 alpha.  Copyright 2004 Ben Rudiak-Gould et al.
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
#include "tweak.h"
#include "tweak/yuy2.h"
#include "tweak/yv12.h"
#include "../../core/videoinfo.h"
#include "../../core/colorspace.h"
#include "../../core/exception/unsupportedcolorspace.h"

//stl include
#include <cmath>                   //for sin, cos


namespace avs { namespace filters {



Tweak::Tweak(PClip const& child, double hue, double sat, double bright, double cont) 
  : clip::onechild::Concrete( child )
  , clip::caching::Concrete( child->GetEnvironment() )
  , Sat( int(sat * 512) )
  , Bright_p16( int(bright) + 16 )
  , Cont( int(cont * 512) )
{
  double Hue = (hue * 3.1415926) / 180.0;

  Sin = int(sin(Hue) * 4096);
  Cos = int(cos(Hue) * 4096);
}


Tweak::Tweak(PClip const& child, Tweak const& other)
  : clip::onechild::Concrete( child )
  , clip::caching::Concrete( child->GetEnvironment() )
  , Sin( other.Sin )
  , Cos( other.Cos )
  , Sat( other.Sat )
  , Bright_p16( other.Bright_p16 )
  , Cont( other.Cont ) { }


PClip Tweak::clone(PClip const& child) const
{
  ColorSpace& space = child->GetVideoInfo()->GetColorSpace();

  switch( space.id() )
  {
  case ColorSpace::I_YUY2: return tweak::YUY2::Create(child, *this);
  case ColorSpace::I_YV12: return tweak::YV12::Create(child, *this);
  }

  throw exception::UnsupportedColorSpace(space);
}


PClip Tweak::Create(PClip const& child, double hue, double sat, double bright, double cont)
{
  ColorSpace& space = child->GetVideoInfo()->GetColorSpace();

  switch( space.id() )
  {
  case ColorSpace::I_YUY2: return tweak::YUY2::Create(child, hue, sat, bright, cont);
  case ColorSpace::I_YV12: return tweak::YV12::Create(child, hue, sat, bright, cont);
  }

  throw exception::UnsupportedColorSpace(space);
}



} } //namespace avs::filters
