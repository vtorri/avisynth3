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


#ifndef __AVS_FILTERS_CHILDCLIP_H__
#define __AVS_FILTERS_CHILDCLIP_H__


//avisynth include
#include "../core/clip.h"



namespace avs { namespace filters {



//////////////////////////////////////////////////////////////////////////////////////
//  ChildClip
//
//  clip subinterface for clip with one child
//
class ChildClip : public virtual Clip
{

public:  //access to child

  virtual PClip GetChild() const = 0;


public:  //child changing clone method

  virtual PClip clone(PClip child) const = 0;


public:  //inner subclasses

  class Forward;   //sub-interface forwarding requests to child
  class Concrete;  //simple implementation

};//ChildClip




//////////////////////////////////////////////////////////////////////////////////////
//  ChildClip::Forward
//
//  forwards all request to its child
//
class ChildClip::Forward : public virtual ChildClip
{

public:  //clip general interface

  virtual PEnvironment GetEnvironment() const { return GetChild()->GetEnvironment(); }
  virtual VideoInfo const& GetVideoInfo() const { return GetChild()->GetVideoInfo(); }

  virtual CPVideoFrame GetFrame(int n) const { return GetChild()->GetFrame(n); }
  virtual void GetAudio(void * buffer, __int64 start, int count) const { GetChild()->GetAudio(buffer, start, count); }

};//ChildClip::Forward



//////////////////////////////////////////////////////////////////////////////////////
//  ChildClip::Concrete
//
//  implementation of the ChildClip interface
//
class ChildClip::Concrete : public virtual ChildClip
{

  PClip child_;


public:  //constructor

  Concrete(PClip child)
    : child_( child->Simplify(child) ) { }


public:  //access to child

  virtual PClip GetChild() const { return child_; }


protected:  //write access

  void SetChild(PClip child) { child_ = child; }

};//ChildClip::Concrete




} } //namespace avs::filters

#endif  //__AVS_FILTERS_CHILDCLIP_H__