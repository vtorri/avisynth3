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


#ifndef __AVS_OWNEDBLOCK_H__
#define __AVS_OWNEDBLOCK_H__

//avisynth includes
#include "block/base.h"
#include "block/align.h"
#include "block/deleter/ownedbase.h"


namespace avs { 

  

///////////////////////////////////////////////////////////////////////////////////////
//  owned_block<align>
//
//  provides same service than Block (from block.h)
//  but the memory is considered owned by an environment
//  which sees its memory usage updated accordingly
//
template <int align>
class owned_block : public block::base<block::deleter::OwnedBase, align>
{

public:  //structors

  owned_block(PEnvironment const& env);

  //normal constructor
  owned_block(PEnvironment const& env, int size, bool recycle);

  template <class Deleter>
  explicit owned_block(Deleter const& deleter)
    :  BaseBlockType( deleter ) { }

  template <int alignOther>
  explicit owned_block(owned_block<alignOther> const& other)
    : BaseBlockType( other ) { }

  //generated copy constructor and destructor are fine


public:  //assignment

  template <int alignOther>
  owned_block<align>& operator=(owned_block<alignOther> const& other)
  {
    return static_cast<owned_block<align>&>( BaseBlockType::operator=(other) );
  }

  //generated operator= is fine
  //swap inherited from superclass


public:  //reset methods

  void reset(int size, bool recycle)
  { 
    owned_block tmp(GetEnvironment(), size, recycle);
    swap(tmp); 
  }

};


//only defined for avs nominal align
template <> owned_block<block::Align>::owned_block(PEnvironment const& env);
template <> owned_block<block::Align>::owned_block(PEnvironment const& env, int size, bool recycle);


} //namespace avs

#endif //__AVS_OWNEDBLOCK_H__
