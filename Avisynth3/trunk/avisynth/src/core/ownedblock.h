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


#ifndef __AVS_OWNEDBLOCK_H__
#define __AVS_OWNEDBLOCK_H__


//avisynth includes
#include "block.h"
#include "runtime_environment.h"


namespace avs {



///////////////////////////////////////////////////////////////////////////////////////
//  OwnedBlock
//
//
//
class OwnedBlock
{

  Block block_;             //underlying owned block
  PEnvironment env_;        //owning environment

  using Block::recycle;


public:  //structors

  //normal constructor
  OwnedBlock(PEnvironment env, int size)
    : block_( env->NewBlock(size) )
    , env_( env ) { }

  //recycling constructor
  OwnedBlock(PEnvironment env, int size, recycle r)   
    : block_( env->NewBlock(size, r) )
    , env_( env ) { }

  //default copy constructor is fine

  //destructor
  ~OwnedBlock()
  {
    if ( unique() )
      env->MemoryFreed( size() );
  }


public:  //assignment

  //default operator= is fine

  void swap(OwnedBlock& other)
  { 
    block_.swap(other.block_); 
    env_.swap(other.env_);
  }


public:  //reset methods

  void reset(int size) { OwnedBlock( env_, size ).swap(*this); }
  void reset(int size, recycle r) { OwnedBlock( env_, size, r ).swap(*this); }


public:  //read access

  PEnvironment GetEnvironment() const { return env_; }

  BYTE * get() const { return block_.get(); }
  int size() const { return env_->SizeOf(block_); }

  bool unique() const { return block_.unique(); }

};//OwnedBlock





} //namespace avs

#endif //__AVS_OWNEDBLOCK_H__