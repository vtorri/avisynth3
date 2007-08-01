// Avisynth v3.0 alpha.  Copyright 2005-2006 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_WINDOW_PTR_H__
#define __AVS_WINDOW_PTR_H__

//stlport include
#include <cassert>


namespace avs {



////////////////////////////////////////////////////////////////////////////////////////:
//  window_ptr<Data>
//
//  Allows manipulation of all plane datas in a friendly manner
//  
//  You fetch all info in one call (you rarely need a only a part anyway)
//  and then access it through the appropriate attribute
//
template <class Data>
struct window_ptr 
{

  Data * ptr;
  long pitch;
  long width;
  long height;


  //structors
  window_ptr(Data * _ptr, long _pitch, long _width, long _height)
    : ptr( _ptr )
    , pitch( _pitch )
    , width( _width )
    , height( _height ) { }

  //compiler generated copy constructor and operator= are fine

  //conversion constructor
  template <typename T>
  window_ptr(window_ptr<T> const& other)
    : ptr( (Data *)other.ptr )
    , pitch( other.pitch * sizeof(T) / sizeof(Data) )
    , width( other.width * sizeof(T) / sizeof(Data) )
    , height( other.height ) 
  { 
    assert( pitch * sizeof(Data) == other.pitch * sizeof(T) );
    assert( width * sizeof(Data) == other.width * sizeof(T) );
  }  


  //data manip methods
  Data * at(int x, int y) const { return ptr + x + y * pitch; }
  Data & operator()(int x, int y) { return *at(x, y); }
  Data & operator[](int x) { return ptr[x]; }


  //movements methods
  void to(int x, int y) { ptr += x + y * pitch; }

  void toTop() { to(0, 1 - height); }
  void toBottom() { to(0, height - 1); }
  //bpp = bytes per pixel
  template <long bpp> void toRight() { to(width - bpp, 0); }
  template <long bpp> void toLeft() { to(width - bpp, 0); }


  //pad methods
  void pad() { ptr += padValue(); }
  void negPad() { ptr += negPadValue(); }
  void skipPad() { ptr += skipPadValue(); }

  long padValue() const { return pitch - width; }
  long negPadValue() const { return -pitch -width; }
  long skipPadValue() const { return pitch * 2 - width; }


  //using Vecteur
  template <class Vect> Data * at(Vect const& vect) const { return at(vect.x, vect.y); }
  template <class Vect> window_ptr<Data>& operator+=(Vect const& vect) { to(vect.x, vect.y); return *this; }

  window_ptr<Data>& operator+=(int x) { ptr += x; return *this; }

};



} //namespace avs

#endif //__AVS_WINDOW_PTR_H__
