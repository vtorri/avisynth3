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



#ifndef __REFCOUNTED_H__
#define __REFCOUNTED_H__




class Counter {

  unsigned count;

public:
  Counter() : count(1) { }

  unsigned long AddRef() { return ++count; }
  unsigned long Release() { return --count; }

  bool IsShared() const { return count > 1; }
};

class COMCounter : public Counter {

  static void IncreaseObjectCount();
  static void DecreaseObjectCount();

public:
  COMCounter() { IncreaseObjectCount(); }

  ~COMCounter() { DecreaseObjectCount(); }
};




/**************************************************************************************
 *  RefCounted class                                                                  *
 **************************************************************************************
  base class for all the refcounted object (IClip, VideoFrame, VideoFrameBuffer...)
  refcount access are limited here (so multithread changes are limited here too)
  in multithread, all methods involving refcount should be made atomic

  Attention: pointers to Refcounted subclasses are never supposed to roam around the world :
             you should use smart_ptr, otherwise you expose yourself to some nasty errors
*/



class RefCounted {

  mutable Counter counter;

public:
  RefCounted() { }
  virtual ~RefCounted() { }  //virtual destructor 

  void AddRef() const { counter.AddRef(); }
  void Release() const { if (counter.Release() == 0) delete this; }  
    
  //used to decide whether clone or steal by smart pointers
  bool IsShared() const { return counter.IsShared(); }  

};


/****************************************************************************************
 * smart pointers classes (refcounting pointers)                                        *
 ****************************************************************************************
  method of conversion between 'smart const T *' and 'smart T *' are provided
  they copy the pointed object if its shared with others, otherwise steal it
*/






template <class T> class smart_ref {

  T * ptr;

  void Set(T * _ptr)
  {
    ptr->Release();
    ptr = _ptr;
  }

public:
  //constructor from a reference (ie normal constructor)
  smart_ref(T & ref) : ptr(&ref) { }
  //constructor from a pointer
  smart_ref(T * _ptr) : ptr(_ptr) { if ( ptr == 0 ) throw std::logic_error("internal error : null pointer"); }
  //copy constructor
  smart_ref(const smart_ref<T>& other) : ptr(other.ptr) { ptr.AddRef(); }

  //destructor
  ~smart_ref() { ptr->AddRef(); }

  const smart_ptr<T>& operator= (const smart_ptr<T>& other) { other.ptr->AddRef(); Set(other.ptr); return *this; }
  const smart_ptr<T>& operator= (T & ref) { Set(&ref); return *this; }

  T * operator->() const { return T; }
  T & operator*() const { return *T; }

  void UnShare() { if ( ptr->IsShared() ) Set(ptr->clone()); }

  void swap(smart_ref<T>& other) { std::swap(ptr, other.ptr); }
};




//base class for the smart pointers classes
template <class T> class smart_ptr_base {

protected:
  T * ptr;

  smart_ptr_base() : ptr(0) { }
  smart_ptr_base(T * _ptr) : ptr(_ptr) { }
  smart_ptr_base (T & _ptr) : ptr(&_ptr) { ptr->AddRef(); } 
  template <class Y> smart_ptr_base(const smart_ptr_base<Y>& other) : ptr(static_cast<T *>(other.Get())) { }  
  template <> smart_ptr_base(const smart_ptr_base<T>& other) : ptr(other.Get()) { }

  void Set(T * _ptr)
  {
    if (ptr) ptr->Release();
    ptr = _ptr;
  }
  
  void Set(T & _ptr)
  {
    _ptr.AddRef();
    Set(&_ptr);
  }

  static T * Clone(const smart_ptr_base<T>& other) { return other.ptr->clone(); }
  static T * StealOrClone(smart_ptr_base<T>& other) { return other.ptr->IsShared() ? Clone(other) : other.Release(); }

  T * Get() const { if (ptr) ptr->AddRef(); return ptr; }  
  T * Release() { T * result = ptr; ptr = 0; return result; } 

public:
  operator bool() const { return ptr != 0; }  //useful in boolean expressions
  bool empty() const { return ptr == 0; }

  void UnShare() { if (ptr && ptr->IsShared()) ptr = ptr->clone(); }

  ~smart_ptr_base() { if (ptr) ptr->Release(); }

};


template <class T> class smart_ptr;

//smart const T * 
template <class T> class smart_ptr_to_cst : public smart_ptr_base<T> {

public:
  smart_ptr_to_cst() { }
  smart_ptr_to_cst(T * _ptr) : smart_ptr_base<T>(_ptr) { }
  smart_ptr_to_cst(T & _ptr) : smart_ptr_base<T>(_ptr) { }
  smart_ptr_to_cst(const T * _ptr) : smart_ptr_base<T>(const_cast<T *>(_ptr)) { }
  smart_ptr_to_cst(const T & _ptr) : smart_ptr_base<T>(const_cast<T &>(_ptr)) { }
  template <class Y> smart_ptr_to_cst(const smart_ptr_to_cst<Y>& other) : smart_ptr_base<T>(other) { }

  smart_ptr_to_cst(smart_ptr<T>& other);
  smart_ptr_to_cst(const smart_ptr<T>& other);

  const smart_ptr_to_cst<T>& operator= (T * _ptr) { Set(_ptr); return *this; }
  const smart_ptr_to_cst<T>& operator= (const T * _ptr) { Set(const_cast<T *>(_ptr)); return *this; }
  const smart_ptr_to_cst<T>& operator= (const smart_ptr_to_cst<T>& other) { Set(other.Get()); return *this; }  
  const smart_ptr_to_cst<T>& operator= (smart_ptr<T>& other);
  const smart_ptr_to_cst<T>& operator= (const smart_ptr<T>& other);

  const T * operator->() const { return ptr; }
  const T & operator* () const { return *ptr; }

  void swap(smart_ptr_to_cst<T>& other) { std::swap(ptr, other.ptr); }
};


//smart T * 
//converting a smart const T * to a smart T * 
template <class T> class smart_ptr : public smart_ptr_base<T> {

public:
  smart_ptr() { }
  smart_ptr(T * _ptr) : smart_ptr_base<T>(_ptr) { }
  smart_ptr(T & _ptr) : smart_ptr_base<T>(_ptr) { }
  template <class Y> smart_ptr(const smart_ptr<Y>& other) : smart_ptr_base<T>(other) { }

  smart_ptr(smart_ptr_to_cst<T>& other) : smart_ptr_base<T>(StealOrClone(other)) { }
  smart_ptr(const smart_ptr_to_cst<T>& other) : smart_ptr_base<T>(Clone(other)) { }

  const smart_ptr<T>& operator= (T * _ptr) { Set(_ptr); return *this; }
  const smart_ptr<T>& operator= (const smart_ptr<T>& other) { Set(other.Get()); return *this; }
  const smart_ptr<T>& operator= (smart_ptr_to_cst<T>& other) { Set(StealOrClone(other)); return *this; }
  const smart_ptr<T>& operator= (const smart_ptr_to_cst<T>& other) { Set(Clone(other)); return *this; }

  T * operator->() const { return ptr; }
  T & operator* () const { return *ptr; }

  void swap(smart_ptr<T>& other) { std::swap(ptr, other.ptr); }

};


template <class T> smart_ptr_to_cst<T>::smart_ptr_to_cst(smart_ptr<T>& other) : smart_ptr_base<T>(StealOrClone(other)) { }
template <class T> smart_ptr_to_cst<T>::smart_ptr_to_cst(const smart_ptr<T>& other) : smart_ptr_base<T>(Clone(other)) { }

template <class T> const smart_ptr_to_cst<T>& smart_ptr_to_cst<T>::operator= (smart_ptr<T>& other) { Set(StealOrClone(other)); return *this; }
template <class T> const smart_ptr_to_cst<T>& smart_ptr_to_cst<T>::operator= (const smart_ptr<T>& other) { Set(Clone(other)); return *this; }



namespace std {

  template <> void swap(smart_ref<T>& a, smart_ref<T>& b) { a.swap(b); }
  template <> void swap(smart_ptr<T>& a, smart_ptr<T>& b) { a.swap(b); }
  template <> void swap(smart_ptr_to_cst<T>& a, smart_ptr_to_cst<T>& b) { a.swap(b); }

};




#endif //define __REFCOUNTED_H__