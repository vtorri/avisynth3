// Avisynth v3.0 alpha.  Copyright 2004 Ben Rudiak-Gould et al.
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


#ifndef __AVS_CACHE_BASE_H__
#define __AVS_CACHE_BASE_H__

//avisynth includes
#include "../cache.h"
#include "../../clip/caching.h"
#include "../environment/base.h"


namespace avs { namespace cache {



/////////////////////////////////////////////////////////////////////////////////////////
//  Base
//
//  Cache subinterface
//
class Base : public Cache
{

  PEnvironment env_;           //owning environment
  FrameMaker const& maker_;

protected:
  
  double dropPriority_;


public:  //structors

  Base(boost::shared_ptr<environment::Base> const& env, FrameMaker const& maker)
    : env_( env )
    , maker_( maker )
    , dropPriority_( 0.0 )
  {
    env->RegisterCache( this );
  }


  virtual ~Base()
  {
    static_cast<environment::Base&>(*env_).UnregisterCache( this );
  }


public:  //Cache interface

  virtual PEnvironment const& GetEnvironment() const { return env_; }


public:  //ordering (by drop priority)

  bool operator< (Base const& other) const { return dropPriority_ < other.dropPriority_; }


private:  //reserved to the env

  virtual void UpdatePriority() = 0;  //update drop priority

  virtual bool Drop() = 0;    //request a frame drop, returns whether one has been performed

  friend class environment::Base;  //so can call the three above


protected:  //implementation helpers

  CPVideoFrame MakeFrame(long n) const { return Cache::MakeFrame(maker_, n); }

};


} } //namespace avs::cache

#endif //__AVS_CACHE_BASE_H__
