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
#include "ownedblock.h"
#include "block_recycler.h"
#include "runtime_environment.h"


namespace avs { namespace block {


OwnedDeleter::OwnedDeleter(PEnvironment env, int size, bool recycle)
  : env_( env )
  , size_( size )
  , recycle_( recycle )
{
  env->MemoryAllocated( size );
}


OwnedDeleter::~OwnedDeleter()
{
  env_->MemoryFreed( size_ );
}


void OwnedDeleter::operator ()(void * ptr) const
{
  Recycler().Return(ptr, size_, recycle_);
}


} //namespace block




  
OwnedBlock::OwnedBlock(PEnvironment env, int size, bool recycle)
: block::base<block::OwnedDeleter>( block::OwnedDeleter(env, size, recycle) ) { }




} //namespace avs