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


//avisynth includes
#include "block.h"

//stl include
#include <map>                       //for map and multimap

//boost include
#include <boost/thread/mutex.hpp>    //for mutex

//C Runtime
#include <malloc.h>


namespace avs {


class Block::Manager
{

  typedef std::map<BYTE *, int> SizeMap;         //maps memory blocks to their size
  typedef std::multimap<int, BYTE *> RecycleMap; //maps size to memory blocks of that size

  static boost::mutex mutex;             //provides synchronisation
  
  static SizeMap sizes;
  static RecycleMap recycle;

  boost::mutex::scoped_lock lock;


public:  //constructor

  Manager()
    : lock( mutex ) { }


public:

  SizeMap& sizeMap() const { return sizes; }
  RecycleMap& recycleMap() const { return recycle; }


};//Block::Manager



Block::Block(int size)
  : block( New(size), &Delete )
{
  Manager().sizeMap()[ get() ] = size;
}


Block::Block(int size, recycle)
{
  Manager manager;

  Manager::RecycleMap::iterator it = manager.recycleMap().find(size);

  BYTE * ptr;

  if ( it != manager.recycleMap().end() )   //if found one to recycle
  {
    BYTE * ptr = it->second;                //take it   
    manager.recycleMap().erase(it);         //remove from recycle map
  }
  else ptr = New(size);                     //else create a new one
  
  block.reset( ptr, &Recycle );             //update shared_ptr
  
  manager.sizeMap()[ get() ] = size;        //register its size

}


int Block::size() const
{ 
  return Manager().sizeMap()[ get() ]; 
}


BYTE * Block::New(int& size)
{
  if ( size < 0 )              //security : if neg
    size = 0;                  //make it zero instead

  BYTE * ptr = (BYTE *) _aligned_malloc(size, Align);

  if ( ptr == NULL )           //bad_alloc if malloc failed
    throw std::bad_alloc();    //else we end up associating a size with NULL...

  return ptr;                         
}


void Block::Delete(void * ptr) throw()
{
  _aligned_free(ptr);                         //free ptr

  Manager().sizeMap().erase( (BYTE *)ptr );  //erase size entry
}


void Block::Recycle(void * ptr) throw()
{
  Manager manager;

  Manager::SizeMap::iterator it = manager.sizeMap().find( (BYTE *)ptr );

  //NB: normally there should be such an entry
  //the test handles the case where an exception in the constructor
  //caused unwinding before size could be registered in the size map
  if ( it == manager.sizeMap().end() )  
    _aligned_free(ptr);                //we delete, what else can we do...
  else
  {
    int size = it->second;             //take the size
    manager.sizeMap().erase(it);       //erase size entry

    //necessary for not throw guarantee
    try { 
      manager.recycleMap().insert( std::make_pair(size, (BYTE *)ptr) );
    }
    catch(std::bad_alloc&)
    {
      _aligned_free(ptr);              //once again, no choice
    }
  }
}

} //namespace avs