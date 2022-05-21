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


//avisynth include
#include "rgbadjust.h"
#include "../../core/videoinfo.h"
#include "../../core/colorspace/rgb/depth8.h"
#include "../../core/videoframe/rgb/depth8.h"
#include "../../core/exception/colorspace/unsupported.h"


namespace avs { namespace filters {



RGBAdjust::RGBAdjust(PClip const& child, double r, double g, double b, double a)
  : clip::onechild::Concrete( child )
  , clip::framemaker::Concrete( child->GetEnvironment() )
  , r_( r )
  , g_( g )
  , b_( b )
  , a_( a )
{
  PColorSpace space = child->GetVideoInfo()->GetColorSpace();

  if ( ! boost::dynamic_pointer_cast<colorspace::rgb::Depth8 const>(space) )
    throw exception::colorspace::Unsupported(space);
}


RGBAdjust::RGBAdjust(PClip const& child, RGBAdjust const& other)
  : clip::onechild::Concrete( child )
  , clip::framemaker::Concrete( child->GetEnvironment() )
  , r_( other.r_ )
  , g_( other.g_ )
  , b_( other.b_ )
  , a_( other.a_ )
{
  PColorSpace space = child->GetVideoInfo()->GetColorSpace();

  if ( ! boost::dynamic_pointer_cast<colorspace::rgb::Depth8 const>(space) )
    throw exception::colorspace::Unsupported(space);
}


PClip RGBAdjust::clone(PClip const& child) const
{
  return PClip( static_cast<Clip *>(new RGBAdjust(child, *this)) );
}



CPVideoFrame RGBAdjust::MakeFrame(PVideoFrame const& source) const
{
  videoframe::rgb::Depth8& frame = dynamic_cast<videoframe::rgb::Depth8&>(*source);

  frame.ApplyToR(r_);
  frame.ApplyToG(g_);
  frame.ApplyToG(b_);
  frame.ApplyToG(a_);

  return source;
}



} } //namespace avs::filters
