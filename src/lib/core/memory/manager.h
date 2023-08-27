// Avisynth v3.0 alpha.  Copyright 2003-2006 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_MEMORY_MANAGER_H__
#define __AVS_MEMORY_MANAGER_H__

//avisynth includes
#include "block.h"                           //so Block is defined
#include "forward.h"                         //for Block and PHolder typedefs
#include "../integer.h"                      //for uint8 and int32 typedefs
#include "../../define.h"                    //for AVS_NOVTABLE

//stl include
#include <memory>                            //so PHolder is defined


namespace avs { namespace memory {



///////////////////////////////////////////////////////////////////////////////////
//  memory::Manager
//
//
//
class AVS_NOVTABLE Manager
{

public:  //structors

  Manager() { }
  virtual ~Manager() { }


public:  //Blocks

  virtual Block MakeBlock(int32 size) = 0;

public:  //memory usage

  //Heap like methods
  virtual uint8 * Acquire(int32 size) = 0;
  virtual void Return(uint8 * ptr, int32 size) = 0;  //no throw

  //memory update
  virtual void MemoryAllocated(int32 size) = 0;
  virtual void MemoryFreed(int32 size) = 0;          //no throw


public:  //helper for Holder::Split

  virtual PHolder Split(int32 splitSize, PHolder& self) const = 0;

};



} } //namespace avs::memory

#endif //__AVS_MEMORY_MANAGER_H__

