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


#include "cache.h"
#include "pclip.h"
#include <algorithm>


CPVideoFrame fetch(int n) { return CPVideoFrame(); }
CPVideoFrame store(int n, CPVideoFrame frame) { return frame; }


CPVideoFrame CacheEverything::fetch(int n)
{
  CacheMap::iterator it = cache.find(n);                    //search entry with key n
  return it != cache.end() ? it->second : CPVideoFrame();   //if found return it, else empty CPVideoFrame
}

CPVideoFrame CacheEverything::store(int n, CPVideoFrame frame)
{
  return cache[n] = frame;
}

bool CacheEverything::drop(int n)
{
  return cache.erase(n) != 0;    //try erasing frame n, which returns erase count...
}


CPVideoFrame RangeCache::store(int n, CPVideoFrame frame)
{
  if ( cache.size() >= size )                                 //we must drop a frame from cache
  {  
    if ( cache.upper_bound(n - (size>>1)) != cache.begin() )  //are there frames to drop in small n ?
      cache.erase(cache.begin());                             //drop from beginning
    else cache.erase(--cache.end());                        //drop from end
  }
  return cache[n] = frame;
};


CPVideoFrame QueueCache::fetch(int n)
{
  for(CacheDeque::iterator it = cache.begin(); it != cache.end(); ++it )    
    if ( it->first == n )               //if found
    {
      rotate(it, it + 1, cache.end());  //put the found frame at end of vector
      return cache.back().second;       //and return it
    }
  return CPVideoFrame();                //case when not found
}

CPVideoFrame QueueCache::store(int n, CPVideoFrame frame)
{
  if ( cache.size() >= size )            //if size reached
    cache.pop_front();                   //erase the oldest (at deque front)
  cache.push_back(make_pair(n, frame));  //store at back
  return frame;
}

bool QueueCache::drop(int n)
{
  for(CacheDeque::iterator it = cache.begin(); it != cache.end(); ++it )  
    if ( it->first == n )    //if found
    {
      cache.erase(it);       //erase it
      return true;           //return true (has an effect)
    }
  return false;              //not found case (no effect)
}





CPVideoFrame Cache::GetCachedFrame(int n, Clip& client, Clip& source)
{

  //first operation : we search all frame caches for frame n
  CPVideoFrame result = sharedCache->fetch(n);   //try shared cache
  ClipToCacheMap::iterator it = clientMap.begin();
  do
  {
    if ( result )                                //if found (non empty CPVideoFrame)
    {
      //search n in frameAges deque, it should be there since frame is found
      IntDeque::iterator i = find(frameAges.begin(), frameAges.end(), n);
      rotate(i, i + 1, frameAges.end());         //make n the most recent frame (move at back)
      return result; 
    }
    if ( it == clientMap.end() )                 //if no more in clientMap to test
      break;                                     //exit the while
    else result = it++->second->fetch(n);        //else try it and move on to next one
  } 
  while ( true );                                //always loop (the break exits the loop)
    
  //if we are here, the frame is not cached
  frameAges.push_back(n);                        //we add n as the latest frame

  FrameCache * targetCache;
  //second operation : we search if client is known from self
  //if found in sharing clients
  if ( find(sharingClients.begin(), sharingClients.end(), &client) != sharingClients.end() ) 
    targetCache = sharedCache;                              //we use sharedCache
  else
  {                                                                       
    it = clientMap.find(&client);                           //search its entry in the client map
    targetCache = it != clientMap.end() ? it->second :      //use the associated cache if found
                           RegisterClient(client, source);  //else create one by registering client
  }

  return targetCache->store(n, source.MakeFrame(n));        //ask source for frame n, store and return it
}


void Cache::DropOldest()
{
  if ( frameAges.empty() )           //if empty
    return;                          //nothing to drop

  int n = frameAges.front();         //get the number of the frame to drop
  frameAges.pop_front();             //remove it from frameAges
  
  if ( sharedCache->drop(n) )         //if succeed to remove in sharedCache
    return;                          //end
  //now search in clientMap
  for(ClipToCacheMap::iterator it = clientMap.begin(); it != clientMap.end(); ++it )  
    if ( it->second->drop(n) )       //as soon as dropped
      return;                        //end

  //if we get there : bug  (exception, assert ?)
}


FrameCache * Cache::RegisterClient(Clip& client, Clip& source)
{
  Clip::CachePolicy policy = client.GetWantedCachePolicy(source); //request client caching policy
  switch(policy.first)
  {
    case Clip::CACHE_ALL:
    case Clip::CACHE_NOTHING:
      //if not in shared cache all mode and it is requested or 2nd client wanting CACHE_NOTHING
      if ( ! sharedCacheAll && (policy.first == Clip::CACHE_ALL || ! sharingClients.empty()) ) 
      {
        sharedCacheAll = true;                                    //we switch to cache all mode
        sharedCache = new CacheEverything();       
      }
      sharingClients.push_back(&client);                          //add client to sharingClients
      return sharedCache;                                         //return sharedCache

    case Clip::CACHE_RANGE:
      return clientMap[&client] = new RangeCache(policy.second);  //store and return new range cache      

    case Clip::CACHE_LAST:
      return clientMap[&client] = new QueueCache(policy.second);  //store and return new queue cache   
  }
  throw std::logic_error("Illegal Cache Policy");                 //just to avoid the warning
}

