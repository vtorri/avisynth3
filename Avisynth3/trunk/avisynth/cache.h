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

#include "videoframe.h"
#include <vector>
#include <map>
#include <utility>
using namespace std;

/********************************************************************
********************************************************************/


//polymorphic frame cache for use by the Cache Filter
class FrameCache {

public:
  FrameCache() { }
  virtual ~FrameCache() { }

  virtual CPVideoFrame fetch(int n) = 0;
  virtual CPVideoFrame store(int n, CPVideoFrame frame) = 0;

};  

//frame cache who keeps frames within a fixed range
//frames out of range are likely to be uncached
class RangeCache : public FrameCache {

  typedef map<int, CPVideoFrame> CacheMap;

  CacheMap cache;
  unsigned size;

public:
  RangeCache(unsigned _size) : size(_size) { }
  
  virtual CPVideoFrame fetch(int n)
  {
    CacheMap::iterator it = cache.find(n);
    return it == cache.end() ? CPVideoFrame() : it->second;
  }

  virtual CPVideoFrame store(int n, CPVideoFrame frame)
  {
    if ( cache.size() >= size )
    { //remove eveything out of range
      cache.erase(cache.lower_bound(n + size), cache.end());
      cache.erase(cache.begin(), cache.upper_bound(n - size));
    }
    return cache[n] = frame;
  }
};

//frame cache who keeps the last used frames
//(fetched frames restart at the beginning of the queue)
class QueueCache : public FrameCache {

  typedef pair<int, CPVideoFrame> CachedVideoFrame;
  typedef vector<CachedVideoFrame> CacheVector;

  CacheVector cache;
  unsigned size;

public:
  QueueCache(unsigned _size) : size(_size) { }

  virtual CPVideoFrame fetch(int n)
  {
    for(CacheVector::iterator it = cache.begin(); it != cache.end(); ++it )    
      if ( it->first == n )
      {
        rotate(it, it + 1, cache.end());  //put the found frame at end of vector
        return cache.back().second;       //and return it
      }
    return CPVideoFrame();                //case when not found
  }

  virtual CPVideoFrame store(int n, CPVideoFrame frame)
  {
    if ( cache.size() >= size )
      cache.erase(cache.begin());  //erase the oldest (at vector front)
    cache.push_back(make_pair(n, frame));
    return frame;
  }
};  

class Clip;

//helper class to implement caches on clips
class Cache {

  typedef vector<FrameCache *> CacheVector;
  typedef map<Clip *, FrameCache *> ClientToCacheMap;

  CacheVector cache;
  ClientToCacheMap clientMap;

  FrameCache * RegisterClient(Clip& client);

public:
  Cache() { }

  //search the frame in the cache(s),
  //eventually request source if not found and update itself as needed
  CPVideoFrame GetCachedFrame(int n, Clip& client, Clip& source);


  ~Cache() { for(CacheVector::iterator it = cache.begin(); it != cache.end(); ++it) delete *it; }
};



#endif  // __Cache_H__