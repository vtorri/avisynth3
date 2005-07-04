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

#ifndef __AVS_BLOCK_H__
#define __AVS_BLOCK_H__

//avisynth includes
#include "block/base.h"
#include "block/align.h"
#include "block/holder.h"


namespace avs {


//forward declaration
namespace block { class Creator; }



///////////////////////////////////////////////////////////////////////////////////////////
//  block_<align>
//
//  Holds an aligned block of memory, with automatic deallocation.
//
//  Two behaviors are provided :
//    - basic behavior : 
//         allocation done at construction and deallocation at destruction
//    - recycling behavior :
//         a list of block of memory (and their size) is maintained.
//      At construction it attempts to reuse one from this list if size match.
//      At destruction, the block of memory is stored (back) into the list.
//
//  The recycling behavior is fully synchronised and can serve multiple threads at once.
//
//
//  Problem: The recycling list only grows and never release memory,
//             even when some sizes are never requested anymore....
//           
template <int align> 
class block_ : public block::base<block::Holder, align>
{

public:  //typedefs

  typedef block_<align> BlockType;
  typedef typename boost::enable_if<block::align_compatible<block::Align, align>, block::Creator>::type Creator;


public:  //structors
  
  block_(int size, bool recycle);

  template <class Holder>
  explicit block_(Holder * holder)
    : BaseBlockType( holder ) { }

  template <int alignOther>
  explicit block_(block_<alignOther> const& other)
    : BaseBlockType( other ) { }

  block_(BaseBlockType const& base)
    : BaseBlockType( base ) { }

  //generated copy constructor and destructor are fine


public:  //assignemnt

  template <int alignOther>
  BlockType& operator=(block_<alignOther> const& other)
  {
    return static_cast<BlockType&>( BaseBlockType::operator=(other) );
  }

  //generated operator= is fine
  //swap inherited from superclass


public:  //misc

  void Reset(int size, Creator create)
  {
    create(size).swap(*this);
  }

  //Split inherited from Superclass

};



namespace block {



//////////////////////////////////////////////////////////////////////////////////////////
//  block::Creator
//
//  This functor is used by the buffer_window template to flatten its interface.
//
//  Its introduction has been necessary when Buffer became a template parameter.
//
class Creator
{

  bool recycle_;


public:  //structors

  Creator(bool recycle = true) : recycle_( recycle ) { }

  template <int align>
    explicit Creator(block_<align> const&, bool recycle = true) : recycle_( recycle ) { }

  //generated destructor is fine


public:  //block creation method

  block_<block::Align> operator()(int size) const
  {
    return block_<block::Align>(size, recycle_);
  }

};



} } //namespace avs::block

#endif //__AVS_BLOCK_H__
