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

//class declarations
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

  int begin_;               //frame offset
  long long audio_begin_;   //audio offset

  CPVideoInfo vi_;          //video info

    
public:  //structors

  Trim(PClip const& child, int begin, int end);
  
  //generated destructor is fine


public:  //clip general interface

  virtual PEnvironment const& GetEnvironment() const { return GetChildEnvironment(); }
  virtual CPVideoInfo GetVideoInfo() const { return vi_; }

  virtual CPVideoFrame GetFrame(int n) const { return GetChildFrame(n + begin_); }
  virtual void GetAudio(void * buffer, long long start, int count) const { GetChildAudio(buffer, start + audio_begin_, count); }


public:  //child changing clone

  virtual PClip clone(PClip const& child) const; 


private:  //Refactorable<KillAudio>

  virtual PClip Refactor(KillAudio const& parent) const;


public:  //read access

  int GetBegin() const { return begin_; }
  int GetEnd() const { return begin_ + GetVideoInfo()->GetFrameCount(); }


public:  //factory method 

  static PClip Create(PClip const& child, int begin, int end) { return PClip( static_cast<Clip *>(new Trim(child, begin, end)) ); }

  struct Creator
  {
    PClip operator()(PClip const& child, int begin, int end) const { return Create(child, begin, end); }
  };

};


} } //namespace avs::filters

#endif //__AVS_FILTERS_TRIM_H__
