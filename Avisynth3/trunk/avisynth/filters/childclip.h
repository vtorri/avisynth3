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
#include "../clip.h"


namespace avs { namespace filters {



/////////////////////////////////////////////////////////////////////////
// ChildClip
//
// instanciable null filter that forwards all requests to a clip child
//
class ChildClip : public virtual Clip
{

  PClip child_;   //the child clip


public:  //constructors

  ChildClip(PClip child)
    : child_( child->Simplify(child) ) { }


public:  //clip interface

  virtual PEnvironment GetEnvironment() const { return child_->GetEnvironment(); }
  virtual const VideoInfo& GetVideoInfo() const { return child_->GetVideoInfo(); }

  virtual CPVideoFrame GetFrame(int n, const CachingClip& client) const
  { 
    return child_->GetFrame(n, client);
  }

  virtual void  GetAudio(BYTE * buf, __int64 start, __int64 count) const
  { 
    child_->GetAudio(buf, start, count);
  }


public:  //cache hints methods

  virtual void Dispatch(const CacheRequest& request) const { child_->Dispatch(request); }
  virtual void Withdraw(const CacheRequest& request) const { child_->Withdraw(request); }


public:  //child changing clone

  virtual Clip * clone(PClip child) const = 0;


public:  //read access

  PClip GetChild() const { return child_; }


protected:  //write access

  void SetChild(PClip child) { child_ = child; }

};


}; }; //namespace avs::filters

#endif  //__AVS_FILTERS_CHILDCLIP_H__