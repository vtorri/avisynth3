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

#ifndef __AVS_BLOCK_H__
#define __AVS_BLOCK_H__

//avisynth includes
#include "block/base.h"
#include "block/align.h"
#include "block/holder.h"


namespace avs {



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

public:  //structors
  
  explicit block_(int size, bool recycle = false);

  template <class Holder>
  explicit block_(Holder * holder)
    :  BaseBlockType( holder ) { }

  template <int alignOther>
  explicit block_(block_<alignOther> const& other)
    : BaseBlockType( other ) { }

  //generated copy constructor and destructor are fine


public:  //assignemnt

  template <int alignOther>
  block_<align>& operator=(block_<alignOther> const& other)
  {
    return static_cast<block_<align>&>( BaseBlockType::operator=(other) );
  }

  //generated operator= is fine
  //swap inherited from superclass


public:  //reset method

  void reset(int size, bool recycle = false) { block_(size, recycle).swap(*this); }

};


//this constructor is only defined for avs nominal align
template <> block_<block::Align>::block_(int size, bool recycle);



} //namespace avs

#endif //__AVS_BLOCK_H__
