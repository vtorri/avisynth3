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


#ifndef __AVS_FILTERS_SPLICE_H__
#define __AVS_FILTERS_SPLICE_H__

//avisynth include
#include "../../clip/manychilds.h"

//stl include
#include <vector>


namespace avs { namespace filters {



////////////////////////////////////////////////////////////////////////////////////
//  Splice
//
//  Joins clips together keeping audio-video in synch
//
class AVS_NOVTABLE Splice : public clip::ManyChilds
{  

  PVideoInfo vi_;
  PEnvironment env_;
  mutable std::vector<PClip> childs_;


public:  //structors

  Splice(CPVideoInfo const& vi, PEnvironment const& env);

  //generated destructor is fine


public:  //clip general interface

  virtual PEnvironment const& GetEnvironment() const { return env_; }
  virtual CPVideoInfo GetVideoInfo() const { return vi_; }


public:  //Simplify methods

  virtual PClip Simplify() const;
  virtual PClip FinalSimplify() const;


public:  //ManyChilds interface

  virtual int GetChildCount() const { return childs_.size(); }
  virtual PClip const& GetChild(int n) const { return childs_[n]; }


public:  //Splice interface

  //if child is A/V compatible with self, add it into the splice, else throw
  virtual void AddChild(PClip const& child) = 0;

  virtual PClip operator[](int n) const { return GetChild(n); }


protected:  //Splice protected interface

  virtual void PushChild(PClip const& child) = 0;


protected:  //implementation helpers

  //checks clip is splice-compatible with self
  void CheckCompatible(PClip const& clip);

  void Merge(Splice const& source);

  void SetFrameCount(int count);
  void SetSampleCount(long long count);

  //attempts to merge with previous clip (if any) and push to childs list
  //returns whether merging could be performed or not
  bool MergingPush(PClip const& child);


public:  //factory methods and functors

  static PClip CreateAligned(PClip const& left, PClip const& right);
  static PClip CreateUnaligned(PClip const& left, PClip const& right);

  struct AlignedCreator
  {
    PClip operator()(PClip const& left, PClip const& right) const { return CreateAligned(left, right); }
  };

  struct UnalignedCreator
  {
    PClip operator()(PClip const& left, PClip const& right) const { return CreateUnaligned(left, right); }
  };

};


} } //namespace avs::filters

#endif  //__AVS_FILTERS_SPLICE_H__
