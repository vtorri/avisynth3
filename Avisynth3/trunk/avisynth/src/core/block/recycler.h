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


#ifndef __AVS_BLOCK_RECYCLER_H__
#define __AVS_BLOCK_RECYCLER_H__

#ifdef _MSC_VER
#pragma warning ( push )           //push warning state
#pragma warning (disable : 4275)   //non dll-interface class 'boost::noncopyable' used as base for dll-interface class 'boost::mutex'
#endif 

//boost include
#include <boost/thread/mutex.hpp>    //for mutex

//stl include
#include <map>                       //for multimap



namespace avs { namespace block {



//////////////////////////////////////////////////////////////////////////////////////////
//  Recycler
//
//  handles reycling of (aligned) memory blocks allocated by Block and OwnedBlock
//
class Recycler
{

  typedef unsigned char BYTE;
  typedef boost::mutex Mutex;
  typedef Mutex::scoped_lock Lock;
  typedef std::multimap<int, void *> RecycleMap; //maps size to memory blocks of that size


  Lock lock_;

  static Mutex mutex_;  
  static RecycleMap map_;  


public:  //constructor

  Recycler()
    : lock_( mutex_ ) { }


public:

  //returns memory to the recycling pool
  void Return(void * ptr, int size, bool recycle);

  //acquires memory from the recycling pool (eventually newly allocated)
  BYTE * Acquire(int size); 

};//Recycler



} } //namespace avs::block

#ifdef _MSC_VER
#pragma warning ( pop )
#endif

#endif //__AVS_BLOCK_RECYCLER_H__
