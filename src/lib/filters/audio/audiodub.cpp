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
#include "audiodub.h"
#include "killaudio.h"
#include "killvideo.h"
#include "../../core/videoinfo.h"


namespace avs { namespace filters {


AudioDub::AudioDub(PClip const& video, PClip const& audio)
  : Concrete( KillAudio::Create(video), KillVideo::Create(audio) ) { }


CPVideoInfo AudioDub::GetVideoInfo() const
{
  PVideoInfo vi = GetVideoChild()->GetVideoInfo();
  vi->MergeAudio( *GetAudioChild()->GetVideoInfo() );
  return vi;
}


PClip AudioDub::FinalSimplify() const
{
  FinalSimplifyChilds();

  //absorbing KillAudio video child
  if ( boost::shared_ptr<KillAudio const> ka = boost::dynamic_pointer_cast<KillAudio const>(GetVideoChild()) )
    SetLeftChild( ka->GetChild() );

  //absorbing KillVideo audio child
  if ( boost::shared_ptr<KillVideo const> kv = boost::dynamic_pointer_cast<KillVideo const>(GetAudioChild()) )
    SetRightChild( kv->GetChild() );

  return shared_from_this();
}


PClip AudioDub::Refactor(KillAudio const& /*parent*/) const
{
  return KillAudio::Create( GetVideoChild() );
}

PClip AudioDub::Refactor(KillVideo const& /*parent*/) const
{
  return KillVideo::Create( GetAudioChild() );
}


PClip AudioDub::Create(PClip const& video, PClip const& audio)
{
  if ( ! audio->GetVideoInfo()->HasAudio() )
    return KillAudio::Create(video);

  if ( ! video->GetVideoInfo()->HasVideo() )
    return KillVideo::Create(audio);

  return PClip( static_cast<Clip *>(new AudioDub(video, audio)) );
}


} } //namespace avs::filters

