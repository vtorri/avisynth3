// Avisynth v3.0 alpha.  Copyright 2003 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_FILTERS_SPLICE_H__
#define __AVS_FILTERS_SPLICE_H__

//avisynth include
#include "../../clip/manychilds.h"

//stl include
#include <vector>

namespace avs { namespace filters {


//declaration
class Trim;



////////////////////////////////////////////////////////////////////////////////////
//  Splice
//
//  Joins clips together keeping audio-video in synch
//
class Splice : public clip::ManyChilds
{  

  typedef std::vector<PClip> ClipVector;
  typedef std::vector<int> VideoSwitchOver;
  typedef std::vector<long long> AudioSwitchOver;

  CPVideoInfo vi_;
  PEnvironment env_;
  
  ClipVector childs_;
  VideoSwitchOver videoSwitchs_;
  AudioSwitchOver audioSwitchs_;


public:  //structors

  Splice(CPVideoInfo const& vi, PEnvironment const& env);

  //generated destructor is fine


public:  //clip general interface

  virtual PEnvironment const& GetEnvironment() const { return env_; }
  virtual CPVideoInfo GetVideoInfo() const { return vi_; }

  virtual CPVideoFrame GetFrame(int n) const;
  virtual void GetAudio(void * buffer, long long start, int count) const;


public:  //Simplify method()

  virtual PClip Simplify() const;


public:  //Splice interface

  //if child is A/V compatible with self, add it into the splice, else throw
  void AddChild(PClip const& child);

};


} } //namespace avs::filters

#endif  //__AVS_FILTERS_SPLICE_H__
