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


#ifndef __AVS_CACHE_CACHELOGIC_H__
#define __AVS_CACHE_CACHELOGIC_H__

//avisynth include
#include "base.h"

//boost include
#include <boost/circular_buffer.hpp>
//actually a boost extension, grab it at http://groups.yahoo.com/group/boost/files/circular_buffer_v3.6.zip


namespace avs { namespace cache {



class CacheLogic
{

  typedef std::pair<int, CPVideoFrame> CachedVideoFrame;

  typedef boost::circular_buffer<int> RequestList;
  typedef boost::circular_buffer<CachedVideoFrame> FrameList;


  RequestList requests_;         //last frames requests
  FrameList frames_;             //cached frames

  float score_;                  //controls number of cached frames
  float inertia_;                //controls speed of cache adaptation
  int minimum_;                  //minimum size of cache to start being effective

  int count_;                    //number of GetCacheFrame calls


public:  //structors

  CacheLogic();

  //generated destructor is fine


public:  //CacheLogic interface

  CPVideoFrame GetCachedFrame(int n);
  void StoreFrame(int n, CPVideoFrame const& frame) { frames_.push_back( std::make_pair(n, frame) ); }

  bool Drop();

  int GetCount() { return count_; } 
  

private:  //implementation

  void UpdateCacheSize();

};


} } //namespace avs::cache

#endif //__AVS_CACHE_CACHELOGIC_H__
