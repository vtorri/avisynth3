// Avisynth v3.0 alpha.  Copyright 2005 David Pierre - Ben Rudiak-Gould et al.
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
#include "block/ownedholder.h"


namespace avs { 


//forward declaration
namespace block { class OwnedCreator; }



///////////////////////////////////////////////////////////////////////////////////////
//  owned_block<align>
//
//  provides same service than Block (from block.h)
//  but the memory is considered owned by an environment
//  which sees its memory usage updated accordingly
//
template <int align>
class owned_block : public block::base<block::OwnedHolder, align>
{

public:  //typedefs

  typedef owned_block<align> BlockType;
  typedef block::base<block::OwnedHolder, align> BaseBlockType; 
  typedef typename boost::enable_if<block::align_compatible<block::Align, align>, block::OwnedCreator>::type Creator;


public:  //structors

  //normal constructor
  owned_block(PEnvironment const& env, int size, bool recycle);

  template <class Holder>
  explicit owned_block(Holder * holder)
    : BaseBlockType( holder ) { }

  template <int alignOther>
  explicit owned_block(owned_block<alignOther> const& other)
    : BaseBlockType( other ) { }

  owned_block(BaseBlockType const& base)
    : BaseBlockType( base ) { }

  //generated copy constructor and destructor are fine


public:  //assignment

  template <int alignOther>
  BlockType& operator=(owned_block<alignOther> const& other)
  {
    return static_cast<BlockType&>( BaseBlockType::operator=(other) );
  }

  //generated operator= is fine
  //swap inherited from superclass


public:  //misc

  void Reset(int size, Creator const& create)
  { 
    create(size).swap(*this);
  }

  //Split inherited from superclass

};



namespace block {



///////////////////////////////////////////////////////////////////////////////////////////////////////
//  block::OwnedCreator
//
//  This functor is used by the buffer_window template to flatten its interface.
//
//  Its introduction has been necessary when Buffer became a template parameter.
//
//  The allocating constructor now takes the buffer associated creator as parameter instead of an env
//  And since we are implicitly convertible from an env, one can still write :
//
//  BufferWindow buffer(dim, env);
//
class OwnedCreator
{

  bool recycle_;
  PEnvironment env_;


public:  //structors

  OwnedCreator(PEnvironment const& env, bool recycle = true)
    : recycle_( recycle )
    , env_( env ) { }

  template <int align>
  explicit OwnedCreator(owned_block<align> const& block, bool recycle = true)
    : recycle_( recycle )
    , env_( block.GetEnvironment() ) { }

  //generated copy constructor and destructor are fine


public:  //operator(), the block creation method

  owned_block<block::Align> operator()(int size) const 
  { 
    return owned_block<block::Align>(env_, size, recycle_); 
  }

};


} } //namespace avs::block

#endif //__AVS_OWNEDBLOCK_H__
