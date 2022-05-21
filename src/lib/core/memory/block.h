// Avisynth v3.0 alpha.  Copyright 2003-2006 David Pierre - Ben Rudiak-Gould et al.
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

#ifndef __AVS_MEMORY_BLOCK_H__
#define __AVS_MEMORY_BLOCK_H__

//avisynth includes
#include "align.h"
#include "holder.h"
#include "forward.h"                     //for PHolder typedef

//boost includes
#include <boost/shared_ptr.hpp>          //so PHolder is defined


namespace avs { namespace memory {



///////////////////////////////////////////////////////////////////////////////////////////
//  block<align>
//
//  Holds an aligned block of memory, with automatic deallocation.
//
template <int align> class block
{

  PHolder holder_;


public:  //structors

  template <class HolderType>
  explicit block( HolderType * holder
                 , typename boost::enable_if<align_compatible<HolderType::Align, align>, void>::type * dummy = NULL
                 )
    : holder_( static_cast<Holder *>(holder) ) { }

  template <int alignOther>
  explicit block( block<alignOther> const& other
                 , typename boost::enable_if<align_compatible<alignOther, align>, void>::type * dummy = NULL
                 )
    : holder_( other ) { }


  //generated copy constructor and destructor are fine


public:  //assignemnt

  template <int alignOther>
  typename boost::enable_if<align_compatible<alignOther, align>, block<align>&>::type
  operator=(block<alignOther> const& other)
  {
    holder_ = other.holder_;
    return *this;
  }

  //generated operator= is fine

  void swap(block<align>& other)
  {
    holder_.swap(other.holder_);
  }


public:  //queries

  int32 Size() const { return holder_->Size(); }
  uint8 * Get() const { return holder_->Get(); }

  bool Unique() const { return holder_.unique() && holder_->Unique(); }


public:  //misc

  block<align> Split(int32 splitSize)
  {
    return block<align>(holder_->Split(splitSize, holder_));
  }

};


//global scope swap
template <int align>
void swap(block<align>& left, block<align> right)
{
  left.swap(right);
}



} } //namespace avs::memory

#endif //__AVS_MEMORY_BLOCK_H__
