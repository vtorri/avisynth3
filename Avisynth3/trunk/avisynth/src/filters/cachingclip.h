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
#include "../core/clip.h"
#include "../core/cache.h"


namespace avs { namespace filters {



//////////////////////////////////////////////////////////////////////////////////
//  CachingClip
//
//  interface for clips who cache (frames)
//
class CachingClip : public virtual Clip
{

public:  //clip general interface

  virtual PEnvironment GetEnvironment() const { return GetCache().GetEnvironment(); }

  virtual CPVideoFrame GetFrame(int n) const { return GetCache().GetFrame(n); }


private:  //CachingClip requirement: have a cache

  virtual Cache& GetCache() const = 0;


private:  //MakeFrame method
  
  //method who creates the requested frame
  //called by the cache, when not cached
  virtual CPVideoFrame MakeFrame(int n) const = 0;


public:  //implementation inner subclasses

  class Concrete;   //you can use it to meet CachingClip requirement (or do it yourself)

};//CachingClip


class CachingClip::Concrete : public virtual CachingClip
{

  mutable Cache cache;


public:  //constructor

  Concrete(PEnvironment env)
    : cache( env ) { }


private:  //GetCache method

  virtual Cache& GetCache() const { return cache; }

};//CachingClip::Concrete





} } //namespace avs::filters

#endif  //__AVS_CACHINGCLIP_H__

