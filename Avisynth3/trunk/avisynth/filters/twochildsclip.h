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


#ifndef __AVS_TWOCHILDSCLIP_H__
#define __AVS_TWOCHILDSCLIP_H__


//avisynth include
#include "videoinfoclip.h"



///////////////////////////////////////////////////////////////////////////////////
// TwoChildsClip
//
// clip with two childs, no cache, own videoinfo
//
class TwoChildsClip : public VideoInfoClip
{

  PClip left_;        //left child
  PClip right_;       //right child


public:  //constructor

  TwoChildsClip(PClip left, PClip right)
    : left_( left->Simplify(left) )
    , right_( right->Simplify(right) ) { }


public:  //clip public interface

  virtual PEnvironment GetEnvironment() const { return left_->GetEnvironment(); }

  virtual void Dispatch(const CacheRequest& request) const
  {
    left_->Dispatch(request);
    right_->Dispatch(request);
  }

  virtual void Withdraw(const CacheRequest& request) const
  {
    left_->Withdraw(request);
    right_->Withdraw(request);
  }


public:  //read access

  PClip GetLeftChild() const { return left_; }
  PClip GetRightChild() const { return right_; }


protected:  //write access

  void SetLeftChild(PClip left) { left_ = left; }
  void SetRightChild(PClip right) { right_ = right; }

};


#endif  //__AVS_TWOCHILDSCLIP_H__