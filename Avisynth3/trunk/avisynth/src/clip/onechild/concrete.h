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


#ifndef __AVS_CLIP_ONECHILD_CONCRETE_H__
#define __AVS_CLIP_ONECHILD_CONCRETE_H__

//avisynth include
#include "../onechild.h"


namespace avs { namespace clip { namespace onechild {



//////////////////////////////////////////////////////////////////////////////////////
//  Concrete
//
//  implementation of the OneChild interface
//
class Concrete : public virtual OneChild
{

  PClip child_;


public:  //constructor

  Concrete(PClip const& child)
    : child_( child->Simplify() ) { }


public:  //access to child

  virtual PClip const& GetChild() const { return child_; }


protected:  //write access

  void SetChild(PClip const& child) { child_ = child; }


protected:  //implementations helper

  PEnvironment const& GetChildEnvironment() const { return child_->GetEnvironment(); }
  CPVideoInfo GetChildVideoInfo() const { return child_->GetVideoInfo(); }

  CPVideoFrame GetChildFrame(int n) const { return child_->GetFrame(n); }
  void GetChildAudio(void * buffer, long long start, int count) const { return child_->GetAudio(buffer, start, count); }

};





} } } //namespace avs::clip::onechild

#endif //__AVS_CLIP_ONECHILD_CONCRETE_H__
