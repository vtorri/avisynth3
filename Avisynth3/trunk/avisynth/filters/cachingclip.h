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


#ifndef __AVS_CACHINGCLIP_H__
#define __AVS_CACHINGCLIP_H__

//avisynth includes
#include "../clip.h"
#include "../cache.h"



//////////////////////////////////////////////////////////////////////////////////
// CachingClip
//
// clip holding a cache
//
class CachingClip : public virtual Clip
{

  mutable Cache cache;
  const CachePolicy policy_;


protected:  //constructors

  CachingClip(PEnvironment env, const CachePolicy& policy)
    : cache( env )
    , policy_( policy ) { }

  CachingClip(const CachingClip& other)
    : cache( other.GetEnvironment() )
    , policy_( other.policy_ ) { }


public:  //clip public interface

  virtual CPVideoFrame GetFrame(int n, const CachingClip& client) const
  {
    return cache.GetFrame(n, client, *this);
  }

  virtual void Dispatch(const CacheRequest& request) const { cache.Dispatch(request); }
  virtual void Withdraw(const CacheRequest& request) const { cache.Withdraw(request); }


protected:
  
  //method who creates the requested frame
  //called by the cache, when not cached
  virtual CPVideoFrame MakeFrame(int n) const = 0;


  //dispatch/withdraw appropriate cache request to a (child) clip
  void DispatchTo(PClip server) const
  { 
    server->Dispatch( boost::make_tuple(this, boost::get<0>(policy_), boost::get<1>(policy_)) ); 
  }
  void WithdrawFrom(PClip server) const
  { 
    server->Withdraw( boost::make_tuple(this, boost::get<0>(policy_), boost::get<1>(policy_)) ); 
  }

  //return a default policy to feed to the constructor
  static const CachePolicy& DefaultCachePolicy() 
  {
    static const CachePolicy defaultPolicy = boost::make_tuple(CACHE_NOTHING, (unsigned)0);
    return defaultPolicy;
  }

};







#endif  //__AVS_CACHINGCLIP_H__

