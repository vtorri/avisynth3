// Avisynth v2.5.  Copyright 2002 Ben Rudiak-Gould et al.
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
//
// As a special exception, the copyright holders of Avisynth give you
// permission to link Avisynth with independent modules that communicate with
// Avisynth solely through the interfaces defined in avisynth.h, regardless of the license
// terms of these independent modules, and to copy and distribute the
// resulting combined work under terms of your choice, provided that
// every copy of the combined work is accompanied by a complete copy of
// the source code of Avisynth (the version of Avisynth used to produce the
// combined work), being distributed under the terms of the GNU General
// Public License plus this exception.  An independent module is a module
// which is not derived from or based on Avisynth, such as 3rd-party filters,
// import and export plugins, or graphical user interfaces.

#ifndef __Cache_H__
#define __Cache_H__

#include "refcounted.h"
#include <vector>
#include <deque>
#include <map>
#include <utility>       //for pair
using namespace std;

#pragma warning(disable:4786)   //disable identifier was truncated to 255 chars....


class VideoFrame;
typedef smart_ptr_to_cst<VideoFrame> CPVideoFrame;


//polymorphic frame cache for use by the Cache class
//default implemention is the for the cache nothing case
class FrameCache {

public:
  FrameCache() { }
  virtual ~FrameCache() { }                                //virtual destructor

  virtual CPVideoFrame fetch(int n);                       //get frame n, return empty CPVideoFrame when not found
  virtual CPVideoFrame store(int n, CPVideoFrame frame);   //store frame n and return it
  virtual bool drop(int n) { return false; }               //drop frame n, return true if has effect
};  


class CacheEverything : public FrameCache {

protected:
  typedef map<int, CPVideoFrame> CacheMap;

  CacheMap cache;

public:
  CacheEverything() { }

  virtual CPVideoFrame fetch(int n);
  virtual CPVideoFrame store(int n, CPVideoFrame frame);
  virtual bool drop(int n);

};

//frame cache who keeps frames within a fixed range
//frames out of range are likely to be uncached
class RangeCache : public CacheEverything {

  unsigned size;   //max number of frames cached

public:
  RangeCache(unsigned _size) : size(_size) { }
  
  virtual CPVideoFrame store(int n, CPVideoFrame frame);
};

//frame cache who keeps the last used frames
//(fetched frames restart at the beginning of the queue)
class QueueCache : public FrameCache {

  typedef pair<int, CPVideoFrame> CachedVideoFrame;
  typedef deque<CachedVideoFrame> CacheDeque;

  CacheDeque cache;
  unsigned size;

public:
  QueueCache(unsigned _size) : size(_size) { }

  virtual CPVideoFrame fetch(int n);
  virtual CPVideoFrame store(int n, CPVideoFrame frame);
  virtual bool drop(int n);
  
};  




class Clip;

//helper class to implement caches on clips
class Cache {

  typedef deque<int> IntDeque;
  typedef vector<Clip *> ClipVector;
  typedef map<Clip *, FrameCache *> ClipToCacheMap;

  IntDeque frameAges;              //used by this::drop to determine which frames are the oldest 

  ClipToCacheMap clientMap;        //associates a client clip to its (non-shared) framecache  

  FrameCache * sharedCache;        //cache used to by all sharing clients
  ClipVector sharingClients;       //vector of sharing clients
  bool sharedCacheAll;             //set when a client request CACHE_ALL, or more than one CACHE_NONE

  static FrameCache cacheNothing;  //the unique cache nothing frame cache needed 

public:
  Cache() : sharedCache(&cacheNothing), sharedCacheAll(false) { }

  ~Cache()
  { 
    for(ClipToCacheMap::iterator it = clientMap.begin(); it != clientMap.end(); ++it)
      delete it->second;        //deletes all frame caches in the client map
    if ( sharedCacheAll )       //if in standard shared mode
      delete sharedCache;       //delete the shared cache
  }

  //search the frame in the cache(s),
  //eventually request source if not found and update itself as needed
  CPVideoFrame GetCachedFrame(int n, Clip& client, Clip& source);

  //drop the oldest frame
  void DropOldest();

private:
  //register an unknown client into self and return the frame cache associated
  FrameCache * RegisterClient(Clip& client, Clip& source);
};



#endif  // __Cache_H__