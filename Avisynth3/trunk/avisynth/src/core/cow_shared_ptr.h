// Avisynth v3.0 alpha.  Copyright 2003 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_COW_SHARED_PTR_H__
#define __AVS_COW_SHARED_PTR_H__

//boost include
#include <boost/shared_ptr.hpp>  //for shared_ptr


namespace avs {



////////////////////////////////////////////////////////////////////////////////
//  cow_shared_ptr<T>
//
//  like shared_ptr but with copy on write semantic
//  ie it guarantees you are the sole owner of the pointed object, at time of use,
//  by implicitly cloning it if it is shared
//
template <class T> class cow_shared_ptr : public boost::shared_ptr<T const>
{

  typedef boost::shared_ptr<T const> parent_type;


  void MakeOwner() const
  {
    if ( ! unique() )
    {
      parent_type const * parent = this;
      const_cast<parent_type *>(parent)->operator=( (*parent)->clone() );
    }
  }

public:  //structors

  cow_shared_ptr() { }

  cow_shared_ptr(boost::shared_ptr<T const> const& ptr)
    : boost::shared_ptr<T const>( ptr ) { }

  cow_shared_ptr(cow_shared_ptr<T> const& other)
    : boost::shared_ptr<T const>( other ) { }


public:  //assignment

  //cow_shared_ptr<T>& operator=(boost::shared_ptr<T const> const& ptr) { ptr_ = ptr; return *this; }


public:  //pointer operators

  T * operator->() const { MakeOwner(); return const_cast<T *>(parent_type::operator->()); }

  T & operator*() const { MakeOwner(); return const_cast<T &>(parent_type::operator*()); }

};


} //namespace avs

#endif //__AVS_COW_SHARED_PTR_H__
