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


/**************************************************************************************
 *  RefCounted class                                                                  *
 **************************************************************************************
  base class for all the refcounted object (IClip, VideoFrame, VideoFrameBuffer...)
  refcount access are limited here (so multithread changes are limited here too)
  in multithread, all methods involving refcount should be made atomic

  Attention: pointers to Refcounted subclasses are never supposed to roam around the world :
             you should use smart_ptr, otherwise you expose yourself to some nasty errors
*/

class smart_ptr_base;

class NotClonable { };  //exception for illegal use of clone

class RefCounted {

  int refcount;

  void AddRef() { ++refcount; }
  void Release() { if (--refcount == 0) delete this; }  
  
  friend class smart_ptr_base;

protected:
  RefCounted() : refcount(0) { }   //refcount starts at 0

  //clone method, used by the conversion between smart_ptr and smart_ptr_to_cst
  //subclass who want to take advantage of this conversion must provide it
  //if its represents constant objects, you can even make it return this (by casting away constness)
  virtual RefCounted * clone() const throw(NotClonable) { throw NotClonable(); }
  
  virtual ~RefCounted() { }  //virtual destructor 

public:
  //used to decide whether clone or steal by smart pointers
  bool isShared() { return refcount > 1; }  


};


/****************************************************************************************
 * smart pointers classes (refcounting pointers)                                        *
 ****************************************************************************************
  method of conversion between 'smart const T *' and 'smart T *' are provided
  they copy the pointed object if its shared with others, otherwise steal it
*/

//base class for the smart pointers classes
//(would have been a template if (stupid) VC6 understood template friend declarations)
class smart_ptr_base {

protected:
  RefCounted * ptr;

  smart_ptr_base() : ptr(NULL) { }
  smart_ptr_base(RefCounted * _ptr) : ptr(_ptr) { if (ptr) ptr->AddRef(); }
  smart_ptr_base(const smart_ptr_base& other) : ptr(other.ptr) { if (ptr) ptr->AddRef(); }  

  inline void Set(RefCounted * newPtr)
  { 
    if (ptr != newPtr) { 
      if (ptr) ptr->Release(); 
      ptr = newPtr; 
      if (ptr) ptr->AddRef(); 
    } 
  } 

  inline void Copy(const smart_ptr_base& other) { Set(other.ptr); }

  inline void Clone(const smart_ptr_base& other) { 
    _ASSERTE(this != &other);
    if (ptr) ptr->Release();
    if (other.ptr) {
      ptr = other.ptr->clone();
      ptr->AddRef();           //clone should never return NULL
    }
    else ptr = NULL;    
  }

  inline void StealOrClone(smart_ptr_base& other)
  {    
    _ASSERTE(this != &other);
    if (ptr) ptr->Release();
    if (other.ptr && other.ptr->isShared()) { //if shared
      ptr = other.ptr->clone();
      ptr->AddRef();          //we make a copy for ourselves (and let the old one live)
    } else {
      ptr = other.ptr;        //we steal ptr
      other.ptr = NULL;
    }
  } 

  inline void SelfClone()
  { 
    if (ptr) {
      RefCounted * newPtr = ptr->clone();
      newPtr->AddRef();       //AddRef should be before Release, clone being allowed to return this  
      ptr->Release();
      ptr = newPtr;
    }
  }

public:
  void release() { if (ptr) { ptr->Release(); ptr = NULL; } } //smart ptr voids itself
  
  operator bool() const { return ptr != NULL; }  //useful in boolean expressions

  ~smart_ptr_base() { if (ptr) ptr->Release(); }

};

template <class T> class smart_ptr;

//smart const T * 
template <class T> class smart_ptr_to_cst : public smart_ptr_base {

public:
  smart_ptr_to_cst() { }
  smart_ptr_to_cst(T* _ptr) : smart_ptr_base(_ptr) { }
  smart_ptr_to_cst(const T *_ptr) : smart_ptr_base(const_cast<T*>(_ptr)) { }
  //casting constructor 
  template <class Y> smart_ptr_to_cst(const smart_ptr_to_cst<Y>& other) : smart_ptr_base(other) { }
  smart_ptr_to_cst(smart_ptr<T>& other);
  smart_ptr_to_cst(const smart_ptr<T>& other);


  void swap(smart_ptr_to_cst<T>& other) { std::swap(ptr, other.ptr); }

  const smart_ptr_to_cst<T>& operator =(T* newPtr) { Set(newPtr); return *this; }
  const smart_ptr_to_cst<T>& operator =(const T* newPtr) { Set(const_cast<T*>(newPtr)); return *this; }
  const smart_ptr_to_cst<T>& operator =(const smart_ptr_to_cst<T>& other) { Copy(other); return *this; }
  const smart_ptr_to_cst<T>& operator =(smart_ptr<T>& other);
  const smart_ptr_to_cst<T>& operator =(const smart_ptr<T>& other);
  
  const T * const operator ->() const { return (T*)ptr; }
  const T& operator *() const { return *((T*)ptr); }

  //comparison operators, operate on the pointed object
  bool operator ==(const smart_ptr_to_cst<T>& other) const { return *((T*)ptr) == *((T*)other.ptr); }
  bool operator < (const smart_ptr_to_cst<T>& other) const { return *((T*)ptr) < *((T*)other.ptr); }

};


//smart T * 
//converting a smart const T * to a smart T * (constructor or operator =) will perform cleanup 
template <class T> class smart_ptr : public smart_ptr_base {

public:
  smart_ptr() { }
  smart_ptr(T* _ptr) : smart_ptr_base(_ptr) { }
  //casting constructor 
  template <class Y> smart_ptr(const smart_ptr<Y>& other) : smart_ptr_base(other) { }  
  smart_ptr(smart_ptr_to_cst<T>& other) { StealOrClone(other); }
  smart_ptr(const smart_ptr_to_cst<T>& other) { Clone(other); }

  void swap(smart_ptr<T>& other) { std::swap(ptr, other.ptr); }
  
  const smart_ptr<T>& operator =(T* newPtr) { Set(newPtr); return *this; }
  const smart_ptr<T>& operator =(const smart_ptr<T>& other) { Copy(other); return *this; }
  const smart_ptr<T>& operator =(smart_ptr_to_cst<T>& other) { StealOrClone(other); return *this; }
  const smart_ptr<T>& operator =(const smart_ptr_to_cst<T>& other) { Clone(other);  return *this; }

  T * const operator ->() const { return (T*)ptr; }
  T& operator *() const { return *((T*)ptr); }

  //comparison operators, operate on the pointed object
  bool operator ==(const smart_ptr<T>& other) const { return *((T*)ptr) == *((T*)other.ptr); }
  bool operator < (const smart_ptr<T>& other) const { return *((T*)ptr) < *((T*)other.ptr); }

  //clone the pointed object, ensuring you don't modify something shared by another smart T *  
  void clone() { SelfClone(); }
};


template <class T> smart_ptr_to_cst<T>::smart_ptr_to_cst(smart_ptr<T>& other) { StealOrClone(other); }
template <class T> smart_ptr_to_cst<T>::smart_ptr_to_cst(const smart_ptr<T>& other) { Clone(other); }

template <class T> const smart_ptr_to_cst<T>& smart_ptr_to_cst<T>::operator =(smart_ptr<T>& other) { StealOrClone(other); return *this; }
template <class T> const smart_ptr_to_cst<T>& smart_ptr_to_cst<T>::operator =(const smart_ptr<T>& other) { Clone(other); return *this; }



#endif //define __REFCOUNTED_H__