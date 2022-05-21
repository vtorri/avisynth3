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
#include "killvideo.h"
#include "../source/voidclip.h"
#include "../../core/videoinfo.h"
#include "../../core/cow_shared_ptr.h"
#include "../../core/exception/novideo.h"


namespace avs { namespace filters {


CPVideoInfo KillVideo::GetVideoInfo() const
{
  PVideoInfo vi = GetChild()->GetVideoInfo();

  vi->KillVideo();

  return vi;
}


CPVideoFrame KillVideo::GetFrame(long /*n*/) const
{
  throw exception::NoVideo();
}



PClip KillVideo::Create(PClip const& child)
{ 
  CPVideoInfo vi = child->GetVideoInfo();

  if ( ! vi->HasAudio() )                              //if child has no audio
    return VoidClip::Create(child->GetEnvironment());  //same as nothing
  if ( ! vi->HasVideo() )                              //if child has no video
    return child;                                      //child is equivalent

  return PClip( static_cast<Clip *>(new KillVideo(child)) ); 
}


} } //namespace avs::filters
