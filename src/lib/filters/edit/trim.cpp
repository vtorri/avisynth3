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


#include "trim.h"
#include "../audio/killaudio.h"


namespace avs { namespace filters {



Trim::Trim(PClip const& child, long begin, long end)
  : clip::onechild::Concrete( child )
  , begin_( begin )
  , audio_begin_( 0 )                 //default for case no sound
{
  PVideoInfo vi = GetChildVideoInfo();

  if ( begin != end || begin < 0)     //begin == end is a special case which only fails if negative
  {
    vi->CheckHasFrame(begin);         //check validity of begin
    vi->CheckHasFrame(end - 1);       //check validity of end
  }

  //now effectively Trim the videoinfo
  vi->SetFrameCount(end - begin);     //framecount updated (check end >= begin too)

  if ( vi->HasAudio() )               //if has audio
  {
    audio_begin_ = vi->AudioSamplesFromFrames(begin);

    long long sampleCount = vi->GetSampleCount() - audio_begin_;   //samples are removed from start
    long long maxCount = vi->AudioSamplesFromFrames(end - begin);  //samples count is limited by length

    vi->SetSampleCount( std::max(0LL, std::min(sampleCount, maxCount)) );  //samplecount updated
  }

  vi_ = vi;                           //save vi to self

  //try absorbing child into self
  if ( std::shared_ptr<Trim const> trim = boost::dynamic_pointer_cast<Trim const>(GetChild()) )
  {
    begin_ += trim->begin_;
    audio_begin_ += trim->audio_begin_;
    SetChild( trim->GetChild() );
  }
}


PClip Trim::clone(PClip const& child) const
{
  return Create(child, GetBegin(), GetEnd());
}



PClip Trim::Refactor(KillAudio const& /*parent*/) const
{
  return clone( KillAudio::Create(GetChild()) );
}


} }  //namespace avs::filters
