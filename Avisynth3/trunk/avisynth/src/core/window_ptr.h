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


#ifndef __AVS_WINDOW_PTR_H__
#define __AVS_WINDOW_PTR_H__



namespace avs {


//declarations
struct Vecteur;
template <class Data> struct window_ptr;

//typedefs
typedef unsigned char BYTE;
typedef window_ptr<BYTE> WindowPtr;
typedef window_ptr<const BYTE> CWindowPtr;



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
  int pitch;
  int width;
  int height;

  //constructor
  window_ptr(Data * _ptr, int _pitch, int _width, int _height)
    : ptr( _ptr )
    , pitch( _pitch )
    , width( _width )
    , height( _height ) { }

  //compiler generated copy constructor and operator= are fine

  Data * at(int x, int y) const { return ptr + x + y * pitch; }
  Data & operator()(int x, int y) { return *at(x, y); }
  Data & operator[](int x) { return ptr[x]; }

  void to(int x, int y) { ptr += x + y * pitch; }
  void pad() { ptr += padValue(); }
  void negpad() { ptr += negPadValue(); }

  int padValue() const { return pitch - width; }
  int negPadValue() const { return -pitch -width; }

  window_ptr<Data>& operator+=(Vecteur const& vect) { to(vect.x, vect.y); return *this; }
  
};//window_ptr<Data>



} //namespace avs

#endif //__AVS_WINDOW_PTR_H__