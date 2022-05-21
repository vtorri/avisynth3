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


#ifndef __AVS_FILTERS_TRIM_H__
#define __AVS_FILTERS_TRIM_H__

//avisynth includes
#include "../../core/videoinfo.h"
#include "../../clip/onechild/concrete.h"
#include "../../clip/onechild/simplifiable.h"


namespace avs { namespace filters {


//class declaration
class KillAudio;



//////////////////////////////////////////////////////////////////////////////////////
// Trim
//
// clip to select a range of frames from a longer clip
//
class Trim : public clip::onechild::Simplifiable<Trim>
           , public clip::Refactorable<KillAudio>
           , public clip::onechild::Concrete
{

  long begin_;               //frame offset
  long long audio_begin_;   //audio offset

  CPVideoInfo vi_;          //video info


private:  //structors

  Trim(PClip const& child, long begin, long end);

  //generated destructor is fine


public:  //clip general interface

  virtual PEnvironment const& GetEnvironment() const { return GetChildEnvironment(); }
  virtual CPVideoInfo GetVideoInfo() const { return vi_; }

  virtual CPVideoFrame GetFrame(long n) const { return GetChildFrame(n + begin_); }
  virtual BYTE * GetAudio(BYTE * buffer, long long start, long count) const { return GetChildAudio(buffer, start + audio_begin_, count); }


public:  //child changing clone

  virtual PClip clone(PClip const& child) const;


private:  //Refactorable<KillAudio>

  virtual PClip Refactor(KillAudio const& parent) const;


public:  //read access

  long GetBegin() const { return begin_; }
  long GetEnd() const { return begin_ + GetVideoInfo()->GetFrameCount(); }


public:  //factory method and functors

  static PClip Create(PClip const& child, long begin, long end)
  {
    return PClip( static_cast<Clip *>(new Trim(child, begin, end)) )->Simplify();
  }

  struct Creator
  {
    PClip operator()(PClip const& child, long begin, long end) const { return Create(child, begin, end); }
  };

  struct OneArgCreator
  {
    PClip operator()(PClip const& child, long begin) const
    {
      return Create(child, begin, child->GetVideoInfo()->GetFrameCount());
    }
  };

  struct NegEndCreator
  {
    PClip operator()(PClip const& child, long begin, long end) const
    {
      return Create(child, begin, child->GetVideoInfo()->GetFrameCount() - end);
    }
  };

};


} } //namespace avs::filters

#endif //__AVS_FILTERS_TRIM_H__
