// Avisynth v3.0 alpha.  Copyright 2004 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_CACHE_CONCRETE_H__
#define __AVS_CACHE_CONCRETE_H__

//avisynth includes
#include "base.h"
#include "../timing/switchingtimer.h"


namespace avs { namespace cache {



/////////////////////////////////////////////////////////////////////////////////////////
//  concrete<CacheLogic, Environment>
//
//  implementation of the cache interface
//
template <class CacheLogic, class Environment> 
class concrete : public Base
{

  typedef clip::Caching Caching;
  typedef boost::shared_ptr<Environment> PEnv;  
  typedef typename Environment::TimerStackMap TimerStackMap;

  typedef typename timing::switching_timer<TimerStackMap> Timer;


  Timer timer_;                  //tracks time spent making the missed frames
  CacheLogic cacheLogic_;
  

public:  //structors

  concrete(PEnv const& env, Caching const& owner)
    : Base( env, owner )
    , timer_( env ) { }

  //generated destructor is fine


public:  //Cache interface

  virtual CPVideoFrame GetFrame(int n)
  {
    if ( CPVideoFrame const * cached = cacheLogic_.GetCachedFrame(n) )
      return *cached;

    typename Timer::Chrono chrono(timer_);

    CPVideoFrame result = MakeFrame(n);
    cacheLogic_.StoreFrame(n, result);
    
    return result;
  }


private:  //called by the env

  virtual void UpdatePriority() { dropPriority_ += 1 / ( timer_.average() * cacheLogic_.GetCount() ); }

  virtual bool Drop() { dropPriority_ = 0; return cacheLogic_.Drop(); }

};



} } //namespace avs::cache

#endif //__AVS_CACHE_CONCRETE_H__
