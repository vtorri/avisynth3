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


//avisynth include
#include "unaligned.h"
#include "../../../core/videoinfo.h"


namespace avs { namespace filters { namespace splice {



void Unaligned::AddChild(PClip const& child)
{
  CheckCompatible(child);

  if ( child->GetVideoInfo()->GetFrameCount() == 0 && child->GetVideoInfo()->GetSampleCount() == 0 )
    return;                                   //give up if has no length

  if ( boost::shared_ptr<Unaligned const> splice = boost::dynamic_pointer_cast<Unaligned const>(child) )
    Merge(*splice);
  else
    PushChild( child );
}


long long Unaligned::PushAudio(AudioSwitch& audio, PClip const& child, bool merging)
{
  return audio.Push(child, merging);
}


} } } //namespace avs::filters::splice
