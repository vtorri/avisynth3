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
#include "runtime_environment.h"


namespace avs {


OwnedBlock::OwnedBlock(PEnvironment env, int size)
  : Block( size )
  , env_( env )
{
  env->MemoryAllocated( Block::size() );  //update env memory use
}


OwnedBlock::OwnedBlock(PEnvironment env, int size, recycle r)
  : Block( size, r )
  , env_( env )
{
  env->MemoryAllocated( Block::size() );  //update env memory use
}


OwnedBlock::~OwnedBlock()
{
  if ( unique() )                         //if last ref to block
    env_->MemoryFreed( size() );          //update env memory use
}


} //namespace avs