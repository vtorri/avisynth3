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
#include "audioswitch.h"
#include "../../../core/videoinfo.h"


namespace avs { namespace filters { namespace splice {



BYTE * AudioSwitch::GetAudio(BYTE * buffer, long long start, long count, Splice const& splice) const
{
  //search where start insert itself in the audio switchs
  SwitchVector::const_iterator it = std::upper_bound(switchs_.begin(), switchs_.end(), start);

  if ( it == switchs_.end() )                                       //if at end (no child or after last one)
    return splice.GetVideoInfo()->GetBlankNoise(buffer, count);     //we make blank noise

  while( count > 0 && it != switchs_.end() )
  {
    PClip const& clip = splice.GetChild(it - switchs_.begin());     //fetch corresponding clip
    long consume = std::min(count, static_cast<long>(*it - start)); //calculate how much samples it should provide

    buffer = clip->GetAudio(buffer, start, consume);                //have clip make his thing (and update buffer)   

    count -= consume;                                               //update parameters
    start += consume;
    ++it;
  }

  return splice.GetVideoInfo()->GetBlankNoise(buffer, count);       //complete with blank if count still > 0 else nothing
}


long long AudioSwitch::Push(PClip const& clip, bool merging)
{
  return Push(clip->GetVideoInfo()->GetSampleCount(), merging);
}

long long AudioSwitch::PushAligned(PClip const& clip, bool merging)
{
  return Push(clip->GetVideoInfo()->AudioSamplesFromFrames(), merging);
}


long long AudioSwitch::Push(long long count, bool merging)
{
  if ( ! switchs_.empty() )
    count += switchs_.back();

  if ( merging )
    switchs_.back() = count;
  else
    switchs_.push_back( count );

  return count;
}


} } } //namespace avs::filters::splice
