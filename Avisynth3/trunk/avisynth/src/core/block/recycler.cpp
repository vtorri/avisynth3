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


//avisynth includes
#include "align.h"
#include "recycler.h"

//C Runtime
#include <malloc.h>


namespace avs { namespace block {


Recycler::Mutex Recycler::mutex_;
Recycler::RecycleMap Recycler::map_;  



void Recycler::Return(void * ptr, int size, bool recycle)
{ 
  if ( recycle )
    map_.insert( std::make_pair(size, ptr) ); 
  else mem_free( ptr);
}


Recycler::BYTE * Recycler::Acquire(int size) 
{
  void * result;

  RecycleMap::iterator it = map_.find(size);

  if ( it != map_.end() )
  {
    result = it->second;
    map_.erase(it);
  }     
  else result = mem_alloc(size);

  return (BYTE *)result;
}


void * Recycler::mem_alloc(int size)
{
#ifdef _MSC_VER
  void * result = _aligned_malloc(size, block::Align);

  if ( result == NULL )
    throw std::bad_alloc();
#else
   BYTE * raw = (BYTE *)malloc(size + block::Align);

  if ( raw == NULL )
    throw std::bad_alloc();

  BYTE * result = (BYTE *)AlignValue(int(value));

  *(result - 1) = result - raw;
#endif

  return result;
}


void Recycler::mem_free(void * ptr)
{
#ifdef _MSC_VER
  _aligned_free(ptr);
#else
  free( (BYTE *)ptr - *((BYTE *)ptr - 1) );
#endif
}


} } //namespace avs::block
