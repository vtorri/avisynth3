// Avisynth v3.0 alpha.  Copyright 2003 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_ENVIRONMENT_BASE_H__
#define __AVS_ENVIRONMENT_BASE_H__

//avisynth include
#include "../runtime_environment.h"

//stl include
#include <vector>


//declaration
namespace avs { namespace cache { class Base; } }


namespace avs { namespace environment {



////////////////////////////////////////////////////////////////////////////////////
//  Base
//
//  partial implementation of the RuntimeEnvironment interface
//
class Base : public RuntimeEnvironment
{

  //memory stuff
  Block temp_;               //internal temp Block
  int memMax_;               //maximum memory usage allowed
  int memUsed_;              //current memory usage


  //Cache stuff
  typedef cache::Base Cache;
  typedef std::vector<Cache *> CacheVector;

  CacheVector caches_;


public:  //structors

  Base(int memMax);

  //generated destructor is fine


public:  //memory stuff

  virtual Block TempBlock(int size);

  virtual void MemoryAllocated(int size);
  virtual void MemoryFreed(int size);

  virtual void MemoryVirtuallyAllocated(int size);


private:  //Cache stuff

  //Cache registration/unregistration
  virtual void RegisterCache(Cache * cache) { caches_.push_back( cache ); }
  virtual void UnregisterCache(Cache * cache);

  friend class cache::Base;  //so can use the two above


private:  //stop/restart timing when attempting memory cleanup

  virtual void InterruptTiming() = 0;
  virtual void RestartTiming() = 0;


private:  //implementation

  //attempts to free memory until memory usage is below max again
  void LowerMemoryUsage();

};


} } //namespace avs::environment

#endif //__AVS_ENVIRONMENT_BASE_H__
