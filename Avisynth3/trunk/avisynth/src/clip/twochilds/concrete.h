// Avisynth v3.0 alpha.  Copyright 2004 Ben Rudiak-Gould et al.
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


#ifndef __AVS_CLIP_TWOCHILDS_CONCRETE_H__
#define __AVS_CLIP_TWOCHILDS_CONCRETE_H__

//avisynth include
#include "../twochilds.h"


namespace avs { namespace clip { namespace twochilds {



/////////////////////////////////////////////////////////////////////////////////////////
//  Concrete
//
//  implementation of the TwoChilds template
//
class Concrete : public virtual TwoChilds
{

  PClip left_;        //left child
  PClip right_;       //right child


public:  //constructor

  Concrete(PClip const& left, PClip const& right)
    : left_( left->Simplify() )
    , right_( right->Simplify() ) { }


public:  //clip general interface

  virtual PEnvironment const& GetEnvironment() const { return left_->GetEnvironment(); }


public:  //read access

  virtual PClip const& GetLeftChild() const { return left_; }
  virtual PClip const& GetRightChild() const { return right_; }


protected:  //write access

  void SetLeftChild(PClip left) { left_ = left; }
  void SetRightChild(PClip right) { right_ = right; }


protected:  //implementations helper

  PEnvironment GetChildEnvironment() const { return left_->GetEnvironment(); }
  
  CPVideoInfo GetLeftVideoInfo() const { return left_->GetVideoInfo(); }
  CPVideoInfo GetRightVideoInfo() const { return right_->GetVideoInfo(); }

  CPVideoFrame GetLeftFrame(int n) const { return left_->GetFrame(n); }
  CPVideoFrame GetRightFrame(int n) const { return right_->GetFrame(n); }

  void GetLeftAudio(void * buffer, __int64 start, int count) const { return left_->GetAudio(buffer, start, count); }
  void GetRightAudio(void * buffer, __int64 start, int count) const { return right_->GetAudio(buffer, start, count); }

};


} } } //namespace avs::clip::twochilds

#endif //__AVS_CLIP_TWOCHILDS_CONCRETE_H__
