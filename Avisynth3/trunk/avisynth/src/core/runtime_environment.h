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



#ifndef __AVS_RUNTIME_ENVIRONMENT_H__
#define __AVS_RUNTIME_ENVIRONMENT_H__

//avisynth includes
#include "block.h"
#include "forward.h"                          //for ColorSpace, Dimension, Blitter, CPVideoFrame
#include "frametype.h"
#include "ownedblock.h"

//boost include
#include <boost/enable_shared_from_this.hpp>  //for enable_shared_from_this


namespace avs {


//declarations
class Cache;                //in cache.h
namespace clip { class Caching; }



//////////////////////////////////////////////////////////////////////////////////
//  RuntimeEnvironment
//
//
//
class RuntimeEnvironment : public boost::enable_shared_from_this<RuntimeEnvironment>
{

public:  //structors

  RuntimeEnvironment() { }
  
  virtual ~RuntimeEnvironment() { }


public:  //memory stuff

  //lend you a (internal) memory block to use as temporary buffer
  //since the same buffer is always reused (when only one of those at once)
  //if you want to keep one for yourself, you must adjust memory usage yourself
  virtual Block TempBlock(int size) = 0;

  //create and return a new OwnedBlock
  //that's how plugin can create owned blocks
  virtual OwnedBlock NewOwnedBlock(int size, bool recycle);

  //methods to update memory usage
  //NB: owned blocks already use them for you
  virtual void MemoryAllocated(int size) = 0;  //no throw
  virtual void MemoryFreed(int size) = 0;      //no throw

  //same as MemoryAllocated but don't trigger mem cleanup
  //instead it considers the increase an unwanted side effect, 
  //the memory not really allocated and expects the reverse decrease soon
  virtual void MemoryVirtuallyAllocated(int size) = 0;  //no throw


public:  //typedef

  typedef clip::Caching Caching;


public:  //various Create or access methods

  //create a cache for the source caching clip
  virtual Cache * CreateCache(Caching const& source) = 0;

  virtual CPVideoFrame CreateFrame(ColorSpace& space, Dimension const& dim, FrameType type);

  //fetch avisynth blitter
  virtual Blitter const& GetBlitter();


public:  //factory method

  static PEnvironment Create(int memMax);

};



} //namespace avs

#endif  //__AVS_RUNTIME_ENVIRONMENT_H__
