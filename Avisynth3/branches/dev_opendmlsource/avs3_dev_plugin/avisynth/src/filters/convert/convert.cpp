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
#include "convert.h"
#include "../../core/videoinfo.h"
#include "../../core/videoframe.h"
#include "../../core/colorspace/external.h"


namespace avs { namespace filters {



Convert::Convert(PClip const& child, PColorSpace const& target)
  : clip::onechild::Concrete( child )
  , clip::framemaker::Concrete( child->GetEnvironment() )
{
  PVideoInfo vi = child->GetVideoInfo();   //start from child VideoInfo
  
  vi->SetColorSpace( target );             //change colorspace (throw on incompatibilities)

  vi_ = vi;                                //save it
}


CPVideoFrame Convert::MakeFrame(PVideoFrame const& source) const
{
  PVideoFrame target = GetEnvironment()->CreateFrame(*vi_, source->GetType());

  ConvertFrame(*source, *target);

  return target;
}


PClip Convert::FromExternal(PClip const& clip, PColorSpace const& target)
{
  boost::shared_ptr<colorspace::External const> space 
      = boost::dynamic_pointer_cast<colorspace::External const>(clip->GetVideoInfo()->GetColorSpace());

  return space->ConvertFrom(clip, target);
}


} } //namespace avs::filters
