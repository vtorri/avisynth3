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


#ifndef __PCLIP_H__
#define __PCLIP_H__


#include "videoinfo.h"
#include "videoframe.h"


#define FRAME_ALIGN 16 //MUST be a power of 2
// Default frame alignment is 16 bytes, to help P4, when using SSE2
#pragma warning( disable : 4290 )  //get rid of C++ exception ... ignored warning



typedef unsigned char BYTE;




/********************************************************************************************
 * Clip and PClip                                                                           *
 ********************************************************************************************
  Base class for all filters.
*/

class Cache;

class Clip : public RefCounted {

public:
  Clip() { }
  
  virtual const VideoInfo& GetVideoInfo() = 0;
  virtual void GetAudio(BYTE * buf, __int64 start, __int64 count) = 0;  // start and count are in samples
 
  //get the frame n for the passed client
  //try cache (if there is any) and forward to MakeFrame if not cached
  virtual CPVideoFrame GetFrame(int n, Clip& client) = 0;

protected:
  //process the frame n (without cache consideration)
  //default implementation: throw a logic_error exception
  //filters using cache should redefine it, others should never call it
  virtual CPVideoFrame MakeFrame(int n) { throw std::logic_error("Illegal MakeFrame Call"); }

  friend class Cache; //so it can use the above

  //two useful helper functions
  //note that it throws errors too if no video (GetFrameCount does it)
  void CheckFrameInBounds(int n) { if ( n < 0 || n >= GetVideoInfo().GetFrameCount() ) ThrowOutOfBoundsException(); }
  void ThrowOutOfBoundsException() { throw std::logic_error("Fatal error: attempted to access an invalid frame"); }

  //another helper function, fill a buffer with blank noise (of the appropriate sample-type/channels)
  void FillWithBlank(BYTE * buf, __int64 count);

  //cache related code
  enum CacheMethod {
    STANDARD_CACHE,
    CACHE_NOTHING,
    CACHE_RANGE,
    CACHE_LAST
  };
  typedef pair<CacheMethod, int> CachePolicy; 

  virtual CachePolicy GetWantedCachePolicy(Clip& server) { return make_pair(STANDARD_CACHE, 0); }
};

typedef smart_ptr<Clip> PClip;  // smart pointer to Clip



// instanciable null filter that forwards all requests to child
class ChildClip : public Clip {

protected:
  PClip child;

  //protected constructor
  ChildClip(PClip _child) : child(_child) { }

  //helper function
  CPVideoFrame GetChildFrame(int n) { return child->GetFrame(n, *this); }

public:
  virtual const VideoInfo& GetVideoInfo() { return child->GetVideoInfo(); }
  virtual void  GetAudio(BYTE * buf, __int64 start, __int64 count) { child->GetAudio(buf, start, count); }

  virtual CPVideoFrame  GetFrame(int n, Clip& client) { return GetChildFrame(n); }

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


#include "cache.h"  

// instanciable null filter with a cache
class GenericVideoFilter : public ChildClip {

  Cache cache;
  
protected:
  //protected constructor
  GenericVideoFilter(PClip _child) : ChildClip(_child) { }

public:
  virtual CPVideoFrame GetFrame(int n, Clip& client) { return cache.GetCachedFrame(n, client, *this); }  
};



#endif //#ifndef __PCLIP_H__