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



#ifndef __AVS_CACHE_H__
#define __AVS_CACHE_H__


//avisynth include
#include "runtime_environment.h"

//boost includes
#include <boost/utility.hpp>      //for noncopyable
#include <boost/shared_ptr.hpp>   //for shared_ptr


namespace avs { 

  
//class declaration
class VideoFrame;

//typedefs
typedef boost::shared_ptr<VideoFrame const> CPVideoFrame;



////////////////////////////////////////////////////////////////////////
//  Cache
//
//
//
class Cache : public boost::noncopyable
{

  PEnvironment env_;        //owning environment


public:  //structors

  Cache(PEnvironment env)
    : env_( env )
  { 
    env_->RegisterCache(this);
  }

  virtual ~Cache()
  { 
    env_->UnRegisterCache(this);
  }


public:  //Cache interface 

  //search the frame in the cache
  //if not found, it has the source clip make it
  virtual CPVideoFrame GetFrame(int n) = 0;

  //feedback method for the env to call
  //drop a cached frame (if any) and adjust caching objectives
  virtual void Drop() = 0;
 

public:  //fetch owning environment method

  PEnvironment GetEnvironment() const { return env_; }


public:  //inner implementation subclass

  class Concrete;

};//Cache




} //namespace avs

#endif  // __AVS_CACHE_H__