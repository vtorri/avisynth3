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


#ifndef __AVS_BLOCK_BASE_H__
#define __AVS_BLOCK_BASE_H__

//avisynth include
#include "../forward.h"                  //for PEnvironment

//boost includes
#include <boost/shared_ptr.hpp>          //for shared_ptr
#include <boost/utility/enable_if.hpp>   //for enable_if_c


namespace avs { namespace block {



//////////////////////////////////////////////////////////////////////////////////////////////
//  block::align_compatible<source, destination>
//
//  helper for block::base
//  used to determine when passing from an align guarantee to another is possible
//
template <int source, int destination> struct align_compatible
{
  static bool const value = source % destination == 0;
};



//////////////////////////////////////////////////////////////////////////////////////////////
//  block::base<BaseHolder, int align>
//
//
//
template <class BaseHolder, int align> class base
{
  
public:  //typedefs

  enum { Align = align };
  typedef base<BaseHolder, align> BaseBlockType;


private:  //member

  boost::shared_ptr<BaseHolder> block_;

  template <class BH, int alignOther> friend class base;    
  //NB: need only friendship with those with same BaseHolder, but it doesn't hurt


public:  //structors

  //construction from a Deleter
  //defined only if the Holder satisifies our alignment requirement
  template <class Holder>
  explicit base( Holder * holder
               , typename boost::enable_if<align_compatible<Holder::Align, Align>, void>::type * dummy = NULL
               )
    : block_( holder ) { }

  //construction from a block using a different align
  //as above, only possible if it satisfies our alignment guarantee
  template <int alignOther>
  explicit base( base<BaseHolder, alignOther> const& other
               , typename boost::enable_if<align_compatible<alignOther, Align>, void>::type * dummy = NULL
               )
    : block_( other.block_ ) { }

  //spawning constructor
  //spawns a holder for this from the one of an already existing block
  base( BaseBlockType const& other, int size )
    : block_( other.block_->spawn(size) ) { }

  //generated copy constructor and destructor are fine


public:  //assignment

  template <int alignOther>
  typename boost::enable_if<align_compatible<alignOther, Align>, BaseBlockType>::type& 
  operator=(base<BaseHolder, alignOther> const& other)
  {
    block_ = other.block_;
    return *this;
  }

  void swap(BaseBlockType& other) { block_.swap( other.block_ ); }


public:  //queries

  BYTE * get() const { return block_->get(); }

  bool unique() const { return block_.unique(); }

  int size() const { return block_->size(); }

  PEnvironment const& GetEnvironment() const { return block_->GetEnvironment(); }

};


//global scope swap
template <class BaseHolder, int align>
void swap(base<BaseHolder, align>& left, base<BaseHolder, align>& right) { left.swap(right); }


//global scope == and !=
template <class BaseHolder, int align>
bool operator==(base<BaseHolder, align> const& left, base<BaseHolder, align> const& right)
{
  return left.get() == right.get();
}

template <class BaseHolder, int align>
bool operator!=(base<BaseHolder, align> const& left, base<BaseHolder, align> const& right)
{
  return left.get() != right.get();
}


} } //namespace avs::block

#endif //__AVS_BLOCK_BASE_H__
