// Avisynth v3.0 alpha.  Copyright 2005 David Pierre - Ben Rudiak-Gould et al.
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


//avisynth includes
#include "base.h"
#include "../cache/base.h"

//boost include
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>


namespace avs { namespace environment {



Base::Base(int memMax)
  : temp_( 1000, false )
  , memMax_( memMax > 0 ? memMax : 10000000 )
  , memUsed_( 0 ) { }



Block Base::TempBlock(int size)
{
  //if internal block is shared or too small
  if ( ! temp_.Unique() || temp_.Size() < size )
    temp_.Reset( size, block::Creator(false) );          //we reset it

  return temp_;
}



void Base::MemoryAllocated(int size)
{
  memUsed_ += size;

  if ( memUsed_ > memMax_ )
    LowerMemoryUsage();
}


void Base::MemoryFreed(int size)
{
  memUsed_ -= size;
}


void Base::MemoryVirtuallyAllocated(int size)
{
  memUsed_ += size;
}


void Base::UnregisterCache(cache::Base * cache)
{
  caches_.erase( std::remove( caches_.begin(), caches_.end(), cache ), caches_.end() );
}


void Base::LowerMemoryUsage()
{
  InterruptTiming();

  CacheVector caches(caches_);

  using namespace boost;
  using namespace boost::lambda;

  std::for_each( caches.begin(), caches.end(), bind(&Cache::UpdatePriority, _1) );

  std::sort(caches.begin(), caches.end(), *_1 < *_2 );

  RestartTiming();
}

} } //namespace avs::environment
