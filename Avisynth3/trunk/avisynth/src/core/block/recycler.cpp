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


//avisynth includes
#include "align.h"
#include "recycler.h"

//C Runtime
#include <malloc.h>


namespace avs { namespace block {



void Recycler::Return(BYTE * ptr, int size, bool recycle)
{ 
  Lock lock(mutex_);

  if ( recycle )
    map_.insert( std::make_pair(size, ptr) ); 
  else mem_free( ptr );
}


Recycler::BYTE * Recycler::Acquire(int size) 
{
  Lock lock(mutex_);

  void * result;

  RecycleMap::iterator it = map_.find(size);

  if ( it != map_.end() )
  {
    result = it->second;
    map_.erase(it);
  }     
  else result = mem_alloc(size);

  return static_cast<BYTE *>(result);
}


void Recycler::Clear()
{
  Lock lock(mutex_);

  for ( RecycleMap::iterator it = map_.begin(); it != map_.end(); ++it )
    mem_free( it->second );

  map_.clear();
}


Recycler::BYTE * Recycler::mem_alloc(int size)
{
#ifdef _MSC_VER
  BYTE * result = (BYTE *)_aligned_malloc(size, block::Align);

  if ( result == NULL )
    throw std::bad_alloc();
#else
   BYTE * raw = (BYTE *)malloc(size + block::Align);

  if ( raw == NULL )
    throw std::bad_alloc();

  BYTE * result = (BYTE *)AlignValue(int(raw + 1));   //align raw + 1 (need a byte to store shift)

  *(result - 1) = result - raw;   //store shift value
#endif //_MSC_VER

  return result;
}


void Recycler::mem_free(BYTE * ptr)
{
#ifdef _MSC_VER
  _aligned_free(ptr);
#else
  free( ptr - *(ptr - 1) );
#endif //_MSC_VER
}


Recycler Recycler::instance;


} } //namespace avs::block
