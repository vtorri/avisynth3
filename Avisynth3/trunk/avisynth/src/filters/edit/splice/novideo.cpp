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
#include "novideo.h"
#include "../../audio/killvideo.h"
#include "../../../core/videoinfo.h"
#include "../../../core/exception/novideo.h"


namespace avs { namespace filters { namespace splice {



CPVideoFrame NoVideo::GetFrame(int /*n*/) const
{
  throw exception::NoVideo();
}


void NoVideo::AddChild(PClip const& child)
{
  CheckCompatible(child);
  
  if ( child->GetVideoInfo()->GetSampleCount() == 0 )  //give up if has no length
    return;

  if ( boost::shared_ptr<NoVideo const> splice = boost::dynamic_pointer_cast<NoVideo const>(child) )
    Merge(*splice);
  else 
    if ( boost::shared_ptr<KillVideo const> killVideo = boost::dynamic_pointer_cast<KillVideo const>(child) )
      PushChild( killVideo->GetChild() );
    else PushChild( child );
}



void NoVideo::PushChild(PClip const& child)
{
  SetSampleCount( audio_.Push(child, MergingPush(child)) );
}


} } } //namespace avs::filters::splice
