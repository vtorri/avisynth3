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


#ifndef __AVS_CACHE_CONCRETE_H__
#define __AVS_CACHE_CONCRETE_H__


//avisynth includes
#include "cache.h"
#include "runtime_environment.h"

//boost include
#include <boost/circular_buffer.hpp>    //for circular_buffer  (NB: actually an extension)

//stl include
#include <utility>                      //for pair



namespace avs {



/////////////////////////////////////////////////////////////////////////////////////////
//  Cache::Concrete
//
//  implementation of the cache interface
//
class Cache::Concrete : public Cache
{

  typedef std::pair<int, CPVideoFrame> CachedVideoFrame;

  typedef boost::circular_buffer<int> RequestList;
  typedef boost::circular_buffer<CachedVideoFrame> FrameList;


  RequestList requests_;               //last frames requests
  FrameList frames_;                   //cached frames
  filters::CachingClip const& owner_;  //back reference to clip owning the cache

  int score_;                          //controls number of cached frames
  int inertia_;                        //controls speed of cache adaptation
  int minimum_;                        //minimum of frames to cache to have hits


public:  //constructor

  Concrete(PEnvironment env, filters::CachingClip const& owner);


public:  //Cache interface

  virtual CPVideoFrame GetFrame(int n); 

  virtual void Drop();


private:  //implementation

  void UpdateSize();

};//Cache::Concrete



} //namespace avs

#endif //__AVS_CACHE_CONCRETE_H__