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


//avisynth includes
#include "cache_concrete.h"
#include "../filters/cachingclip.h"

//boost include
#include <boost/bind.hpp>               //for bind

//stl includes
#include <algorithm>                    //for find, find_if
#include <functional>                   //for equal_to


namespace avs {



Cache::Concrete::Concrete(PEnvironment env, filters::CachingClip const& owner)
  : Cache( env )
  , requests_( 50 )
  , frames_( 0 )
  , owner_( owner )
  , score_( 0 )
  , inertia_( 1 )
  , minimum_( 100 ) { }



CPVideoFrame Cache::Concrete::GetFrame(int n)
{

  FrameList::iterator fit = std::find_if( frames_.begin(), frames_.end(),
    boost::bind<bool>( std::equal_to<int>(), boost::bind( &CachedVideoFrame::first, _1), n ) );

  if ( fit != frames_.end() )     //if was cached
    return fit->second;           //return it

  RequestList::iterator rit = std::find( requests_.begin(), requests_.end(), n );
 
  if ( rit == requests_.end() )   //if not found
    requests_.push_front(n);      //store n as the last request (newest are at buffer front)
  else
  {
    //that's the size the cache should have had for n to be cached
    int depth = 1 + (rit - requests_.end());    

    if ( depth < minimum_ ) 
      minimum_ = depth;

    score_ += 1000 / ( inertia_ * depth );

    UpdateSize(); 
  }

  CPVideoFrame result = owner_.MakeFrame(n);       //owning clip makes frame n 
  frames_.push_back( std::make_pair(n, result) );  //it is stored
  return result;                                   //and returned
}


void Cache::Concrete::Drop()
{
  if ( ! frames_.empty() )
  {
    score_ -= 1000;
    inertia_ += 1;

    UpdateSize();
  }
}


void Cache::Concrete::UpdateSize()
{
  int size = score_ / 1000;
  if ( size < minimum_ )      //if too small to do any good
    size = 0;                 //we cache nothing instead

  frames_.set_capacity( size );
}



} //namespace avs