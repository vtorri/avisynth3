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


#ifndef __CLIP_HIERARCHY_H__
#define __CLIP_HIERARCHY_H__


#include "clip.h"
#include "cache.h"        //which includes vector
#include "videoinfo.h"
#include "videoframe.h


// instanciable null filter that forwards all requests to child
class ChildClip : public Clip {

  PClip child;

protected:
  //protected constructor
  ChildClip(PClip _child) : child(_child) { }

  //helper function
  CPVideoFrame GetChildFrame(int n) { return child->GetFrame(n, *this); }

public:
  virtual PEnvironment GetEnvironment() { return child->GetEnvironment(); }
  virtual const VideoInfo& GetVideoInfo() { return child->GetVideoInfo(); }

  virtual CPVideoFrame GetFrame(int n, Clip& client) { return GetChildFrame(n); }

  virtual void  GetAudio(BYTE * buf, __int64 start, __int64 count) { child->GetAudio(buf, start, count); }

  //additional method
  PClip GetChild() const { return child; }

};



class MultiChildClip : public Clip {

  const VideoInfo vi;

protected:
  typedef vector<PClip> ClipVector;

  ClipVector childs;  //child vector

  //protected constructor
  MultiChildClip(const ClipVector _childs, const VideoInfo& _vi) : childs(_childs), vi(_vi) { }

  //helper function
  CPVideoFrame GetChildFrame(int clip, int n) { return childs[clip]->GetFrame(n, *this); }

public:
  virtual const VideoInfo& GetVideoInfo() { return vi; }

};


//instanciable null filter forwarding to child, without a cache
//but with a modified videoinfo
//subclass from this for filters who just perform frames reordering
class EditFilter : public ChildClip {

  const VideoInfo vi;

protected:
  EditFilter(PClip child, const VideoInfo& _vi) : ChildClip(child), vi(_vi) { }

public:
  virtual const VideoInfo& GetVideoInfo() { return vi; }

};



// instanciable null filter with a cache
class GenericVideoFilter : public ChildClip {

  Cache cache;
  
protected:
  //protected constructor
  GenericVideoFilter(PClip _child) : ChildClip(_child) { }

public:
  virtual CPVideoFrame GetFrame(int n, Clip& client) { return cache.GetFrame(n, client, *this); }  
};








#endif //__CLIP_HIERARCHY_H__