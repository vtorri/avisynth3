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


#ifndef __AVS_SMART_REF_H__
#define __AVS_SMART_REF_H__


//stl include
#include <string>

//boost include
#include <boost/shared_ptr.hpp>

//class declaration
template <class T> class smart_ref_cow;



///////////////////////////////////////////////////////////////////////////
//  bad_smart_ref
//
//  exception throw by smart_ref (and smart_ref_cow)
//  when attempting to make them point to NULL
//
class bad_smart_ref : public std::logic_error
{

public:
  bad_smart_ref() : std::logic_error("") { }


  virtual const char * what() const { return "bad smart_ref"; }

};



///////////////////////////////////////////////////////////////////////////
//  smart_ref<T>
//
//  templated smart pointer class
//  behaves like a T *, except it is enforced non NULL (may throw)
//  while automically managing T lifetime (the embedded shared_ptr does)
//
template <class T> class smart_ref
{

  boost::shared_ptr<T> ptr_;    //underlying shared_ptr


public:  //constructors

  //construction from a raw pointer
  smart_ref(T * ptr) : ptr_( ptr )
  {
    if ( ! ptr )
      throw bad_smart_ref();
  }

  //construction from a shared_ptr (with potential upcasting)
  template <class U>
  smart_ref(const boost::shared_ptr<U>& ptr) : ptr_( ptr )
  {
    if ( ! ptr_ )
      throw bad_smart_ref();
  }

  //upcasting constructor from another smart_ref
  template<class U>
  smart_ref(const smart_ref<U>& other) : ptr_( other ) { }


  //conversion from a smart_ref_cow (with upcasting)
  template<class U>
  smart_ref(const smart_ref_cow<U>& other);


public:  //assignement

  //from a raw pointer
  smart_ref<T>& operator=(T * ptr)
  {
    if ( ! ptr )
      throw bad_smart_ref();
    ptr_ = ptr;
    return *this;
  }

  //from a shared_ptr (with upcasting)
  template <class U>
  smart_ref<T>& operator=(const boost::shared_ptr<U>& ptr)
  {
    if ( ! ptr )
      throw bad_smart_ref();
    ptr_ = ptr;
    return *this;
  }

  //from another smart_ref
  template <class U>
  smart_ref<T>& operator=(const smart_ref<U>& other)
  {
    ptr_ = other;
    return *this;
  }

  //from a smart_ref_cow
  template <class U>
  smart_ref<T>& operator=(const smart_ref_cow<U>& other);


public:  //pointers operators

  T * operator->() const { return ptr_.operator->(); }
  T & operator*() const { return ptr_.operator*(); }

};

#endif  //__AVS_SMART_REF_H__