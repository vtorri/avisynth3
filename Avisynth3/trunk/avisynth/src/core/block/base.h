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
#include <boost/mpl/and.hpp>             //for mpl::and_
#include <boost/shared_ptr.hpp>          //for shared_ptr
#include <boost/utility/enable_if.hpp>   //for enable_if_c
#include <boost/type_traits/is_base_and_derived.hpp>   //for is_base_and_derived


namespace avs { namespace block {



/////////////////////////////////////////////////////////////////////////////////////
//  block::base<BaseDeleter, int align>
//
//
//
template <class BaseDeleter, int align> class base
{
  
public:  //typedefs

  enum { Align = align };
  typedef unsigned char BYTE;
  typedef base<BaseDeleter, align> BaseBlockType;

  //helper struct for the below enable_ifs
  template <int alignOther> struct compatible
  {
    static bool const value = alignOther % Align == 0;
  };


private:  //member

  boost::shared_ptr<BYTE> block_;

  template <class BD, int alignOther> friend class base;    
  //NB: need only friendship with those with same BaseDeleter, but it doesn't hurt


public:  //structors

  //construction from a Deleter
  //instantation is blocked when the alignment guarantee is not satisfied and/or Deleter doesn't subclass BaseDeleter
  template <class Deleter>
  explicit base( Deleter const& deleter
               , typename boost::enable_if
                    < compatible<Deleter::Align>
                    , typename boost::enable_if<boost::is_base_and_derived<BaseDeleter, Deleter>, void>::type 
                    >::type * dummy = NULL
               )
    : block_( deleter.Acquire(), deleter ) { }

  //construction from a block using a different align
  //as above, only possible if it satisfies our alignment guarantee
  template <int alignOther>
  explicit base( base<BaseDeleter, alignOther> const& other
               , typename boost::enable_if<compatible<alignOther>, void>::type * dummy = NULL
               )
    : block_( other.block_ ) { }

  //generated copy constructor and destructor are fine


public:  //assignment

  template <int alignOther>
  typename boost::enable_if<compatible<alignOther>, BaseBlockType>::type& 
  operator=(base<BaseDeleter, alignOther> const& other)
  {
    block_ = other.block_;
    return *this;
  }

  void swap(BaseBlockType& other) { block_.swap( other.block_ ); }


public:  //queries

  BYTE * get() const { return block_.get(); }

  bool unique() const { return block_.unique(); }

  int size() const { return GetBaseDeleter().size(); }

  PEnvironment const& GetEnvironment() const { return GetBaseDeleter().GetEnvironment(); }


protected:

  BaseDeleter const& GetBaseDeleter() const { return *boost::get_deleter<BaseDeleter>(block_); }

};


//global scope swap
template <class BaseDeleter, int align>
void swap(base<BaseDeleter, align>& left, base<BaseDeleter, align>& right) { left.swap(right); }


//global scope == and !=
template <class BaseDeleter, int align>
bool operator==(base<BaseDeleter, align> const& left, base<BaseDeleter, align> const& right)
{
  return left.get() == right.get();
}

template <class BaseDeleter, int align>
bool operator!=(base<BaseDeleter, align> const& left, base<BaseDeleter, align> const& right)
{
  return left.get() != right.get();
}


} } //namespace avs::block

#endif //__AVS_BLOCK_BASE_H__
