// Avisynth v3.0 alpha.  Copyright 2003 Ben Rudiak-Gould et al.
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


Trim::Trim(PClip child, int begin, int end)
  : EditFilter( child )
  , begin_( begin )
  , audio_begin_( 0 )               //default for case no sound
{
  VideoInfo& vi = GetVideoInfo();

  vi.CheckHasRange(begin, end);     //throw appropriate exception
  
  //now efectively Trim the videoinfo
  vi.SetFrameCount(end - begin);    //framecount updated

  if ( vi.HasAudio() )              //if has audio
  {
    audio_begin_ = vi.SamplesFromFrames(begin);

    __int64 samplecount = vi.GetSampleCount() - audio_begin_;   //samples are removed from start
    __int64 maxcount = vi.SamplesFromFrames(end - begin);       //samples count is limited by length
    
    vi.SetSampleCount( std::max(0, std::min(samplecount, maxcount)) );  //samplecount updated
  }

  //try absorbing child into self
  if ( boost::shared_ptr<const Trim> trim = dynamic_pointer_cast<const Trim>(GetChild()) )
  {
    begin_ += trim->begin_;
    audio_begin_ += trim->audio_begin_;
    SetChild( trim->GetChild() );
  } 
}


Clip * Trim::clone(PClip child) const
{
  return new Trim(child, GetBegin(), GetEnd());
}



PClip Trim::Refactor(const KillAudio& parent) const
{
  return clone( parent.clone(GetChild()) );
}


}; };  //namespace avs::filters