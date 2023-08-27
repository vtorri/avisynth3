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


#ifndef __AVS_UTILITY_VALUECACHE_H__
#define __AVS_UTILITY_VALUECACHE_H__

//boost include
#include <boost/weak_ptr.hpp>


namespace avs { namespace utility {


namespace synchronizer { struct None { }; }



////////////////////////////////////////////////////////////////////////////////////////////
//  ValueCache<T, Synchronizer, Creator>
//
//
//
template <typename T, typename Synchronizer = synchronizer::None, typename Creator = T * (*)()>
class value_cache
{

  Creator create_;
  mutable std::weak_ptr<T> cached_;


public:  //structors

  value_cache(Creator create)
    : create_( create ) { }

  //generated destructor is fine


public:  //interface

  std::shared_ptr<T> Get() const
  {
    Synchronizer synchronizer;

    std::shared_ptr<T> result = cached_.lock();
    if ( ! result )
      cached_ = result = std::shared_ptr<T>( create_() );

    return result;
  }

};



} } //namespace avs::utility

#endif //__AVS_UTILITY_VALUECACHE_H__
