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
//
// Linking Avisynth statically or dynamically with other modules is making a
// combined work based on Avisynth.  Thus, the terms and conditions of the GNU
// General Public License cover the whole combination.


#ifndef __AVS_OWNEDBLOCK_H__
#define __AVS_OWNEDBLOCK_H__

//avisynth includes
#include "block/base.h"
#include "block/owneddeleter.h"


namespace avs { 

  

///////////////////////////////////////////////////////////////////////////////////////
//  OwnedBlock
//
//  provides same service than Block (from block.h)
//  but the memory is considered owned by an environment
//  which sees its memory usage updated accordingly
//
class OwnedBlock : public block::base<block::OwnedDeleter>
{

public:  //structors

  //normal constructor
  OwnedBlock(PEnvironment env, int size, bool recycle);

  //generated copy constructor and destructor are fine


public:  //reset methods

  void reset(int size, bool recycle)
  { 
    OwnedBlock( GetEnvironment(), size, recycle ).swap(*this); 
  }


public:  //read access

  PEnvironment const& GetEnvironment() const { return GetDeleter().GetEnvironment(); }


};//OwnedBlock





} //namespace avs

#endif //__AVS_OWNEDBLOCK_H__