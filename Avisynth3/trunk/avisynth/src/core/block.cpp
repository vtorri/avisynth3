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


//avisynth include
#include "block.h"

//stl include
#include <map>                       //for map and multimap

//boost include
#include <boost/thread/mutex.hpp>    //for mutex

//C Runtime
#include <malloc.h>


namespace avs {



class Block::Recycler
{

  typedef std::multimap<int, BYTE *> RecycleMap; //maps size to memory blocks of that size

  static boost::mutex mutex;             //provides synchronisation
  
  static RecycleMap recycleMap;

  boost::mutex::scoped_lock lock;


public:  //constructor

  Recycler()
    : lock( mutex ) { }


public:

  void Return(int size, BYTE * ptr) const { recycleMap.insert( std::make_pair(size, ptr) ); }

  boost::shared_ptr<BYTE> Acquire(int size, bool recycle) const
  {
    boost::shared_ptr<BYTE> result;

    RecycleMap::iterator it = recycleMap.find(size);

    if ( it != recycleMap.end() )
    {
      result.reset( it->second, Deleter(size, true) );
      recycleMap.erase(it);
    }     
    else 
    {
      BYTE * ptr = (BYTE *)_aligned_malloc(size, Align);

      if ( ptr == NULL )
        throw std::bad_alloc();

      result.reset( ptr, Deleter(size, recycle) );
    }

    return result;
  }



};//Block::Recycler



Block::Block(int size, bool recycle)
  : block( Recycler().Acquire(size, recycle) ) { }


void Block::Deleter::operator ()(void * ptr) const
{
  if ( recycle_ )
    Recycler().Return(size_, (BYTE *)ptr);
  else _aligned_free(ptr);
}
  






} //namespace avs