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
#include "noaudio.h"
#include "../../audio/killaudio.h"
#include "../../../core/videoinfo.h"
#include "../../../core/exception/noaudio.h"


namespace avs { namespace filters { namespace splice {



BYTE * NoAudio::GetAudio(BYTE * /*buf*/, long long /*start*/, int /*count*/) const
{
  throw exception::NoAudio();
}


void NoAudio::AddChild(PClip const& child)
{
  CheckCompatible(child);
  
  if ( child->GetVideoInfo()->GetFrameCount() == 0 )  //give up if has no length
    return;

  if ( boost::shared_ptr<NoAudio const> splice = boost::dynamic_pointer_cast<NoAudio const>(child) )
    Merge(*splice);
  else 
    if ( boost::shared_ptr<KillAudio const> killAudio = boost::dynamic_pointer_cast<KillAudio const>(child) )
      PushChild( killAudio->GetChild() );
    else PushChild( child );
}


void NoAudio::PushChild(PClip const& child)
{
  SetFrameCount( video_.Push(child, MergingPush(child)) );
}


} } } //namespace avs::filters::splice
