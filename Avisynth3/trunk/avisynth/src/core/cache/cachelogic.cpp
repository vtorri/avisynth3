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


//avisynth include
#include "cachelogic.h"

//boost include
#include <boost/lambda/bind.hpp>  
#include <boost/lambda/lambda.hpp>  //for lambda

//stl include
#include <algorithm>        //for find, find_if
#include <functional>       //for equal_to


namespace avs { namespace cache {


CacheLogic::CacheLogic()
  : requests_( 50 )
  , frames_( 0 )
  , score_( 0 )
  , inertia_( 1.0 )
  , minimum_( 1000 )
  , count_( 1 ) { }


CPVideoFrame CacheLogic::GetCachedFrame(int n)
{
  ++count_;

  using namespace boost;
  using namespace boost::lambda;

  FrameList::iterator fit = std::find_if( frames_.begin(), frames_.end(), bind<int>( &CachedVideoFrame::first, _1) == n );

  if ( fit != frames_.end() )     //if was cached
    return fit->second;           //return it

  RequestList::iterator rit = std::find( requests_.begin(), requests_.end(), n );
 
  if ( rit == requests_.end() )   //if not found
    requests_.push_front(n);      //store n as the last request (newest are at buffer front)
  else
  {
    //that's the size the cache should have had for n to be cached
    int depth = requests_.end() - rit;    

    if ( depth < minimum_ ) 
      minimum_ = depth;

    score_ += 1 / ( inertia_ * depth );

    UpdateCacheSize(); 
  }

  return CPVideoFrame();
}




bool CacheLogic::Drop()
{
  if ( ! frames_.empty() )
  {
    score_ -= 1.0f;
    inertia_ *= 1.1f;

    UpdateCacheSize();

    return true;
  }
  return false;
}


void CacheLogic::UpdateCacheSize()
{
  int size = int( score_ );
  if ( size < minimum_ )      //if too small to do any good
    size = 0;                 //we cache nothing instead

  frames_.set_capacity( size );
}




} } //namespace avs::cache
