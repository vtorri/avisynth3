// Avisynth v3.0 alpha.  Copyright 2002 Ben Rudiak-Gould et al.
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


#ifndef __MEMORYMANAGER_H__
#define __MEMORYMANAGER_H__

#include <map>                //for multimap
#include <boost/utility.hpp>  //for noncopyable
#include "environment.h"


typedef unsigned char BYTE;



class MemoryManager  {

  typedef multimap<int, BYTE *> AllocationMap;

  static AllocationMap recycle;

  //memory allocations and deallocations methods
  //private so they are only done through MemoryPiece
  static BYTE * alloc(int& size);
  static void free(BYTE *, int size);

  friend class MemoryPiece;  //so it can use the two above

public:

  static void clear()
  {
    for(AllocationMap::iterator it = recycle.begin(); it != recycle.end(); )
      delete[] it->second;
    recycle.clear();
  }
  
};


class MemoryPiece : public boost::noncopyable {
 
  int size_;
  BYTE * data;

public:
  MemoryPiece(int _size) : size_(_size), data(MemoryManager::alloc(size_)) { }
  ~MemoryPiece() { MemoryManager::free(data, size_); }

  int size() const { return size_; }
  BYTE * get() { return data; }
  const BYTE * get() const { return data; } 

};



class OwnedMemoryPiece : public MemoryPiece {

  PEnvironment env;       //owning environment

public:
  OwnedMemoryPiece(int _size, PEnvironment _env)
    : MemoryPiece(_size), env(_env) { env->MemoryAllocated(size()); }

  ~OwnedMemoryPiece() { env->MemoryFreed(size()); }      //destructor, report env no longer use memory

  PEnvironment GetEnvironment() const { return env; }    //return owning env  

  void SetEnvironment(PEnvironment _env)                 //change owning env, updating memory usage
  { 
    if ( env != _env)
    {
      env->MemoryFreed(size());
      env = _env;
      env->MemoryAllocated(size());
    }
  }
};



#endif  //__MEMORYMANAGER_H__