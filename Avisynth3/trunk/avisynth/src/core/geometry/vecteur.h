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


#ifndef __AVS_VECTEUR_H__
#define __AVS_VECTEUR_H__

//avisynth include
#include "dimension.h"

//boost include
#include <boost/operators.hpp>

//stl include
#include <algorithm>           //for std::min and std::max


namespace avs {



////////////////////////////////////////////////////////////////////////////////////////////
//  vecteur<T>
//
//
//
template <typename T>
struct vecteur
     : boost::addable<vecteur<T>                          //defines vecteur + vecteur
     , boost::subtractable<vecteur<T>                     //defines vecteur - vecteur
     , boost::right_shiftable<vecteur<T>, dimension<T> >  //defines vecteur >> dimension (inclusion)
       > >
{

  T x, y;   

  //constructors
  vecteur() : x( 0 ), y( 0 ) { }
  vecteur(T _x, T _y) : x( _x ), y( _y ) { }
  vecteur(vecteur<T> const& other) : x( other.x ), y( other.y ) { }
  
  vecteur(dimension<T> const& dim)  : x( dim.GetWidth() ), y( dim.GetHeight() ) { }

  
  //assignment
  vecteur& operator=(vecteur<T> const& other)
  { 
    x = other.x; 
    y = other.y; 
    return *this; 
  }
  
  void swap(vecteur<T>& other) 
  { 
    using std::swap;
    swap(x, other.x); 
    swap(y, other.y); 
  }

  
  //classic vector operators
  vecteur operator-() const { return vecteur<T>(-x, -y); }
  vecteur& operator+=(vecteur<T> const& other) { x += other.x; y += other.y; return *this; }
  vecteur& operator-=(vecteur<T> const& other) { x -= other.x; y -= other.y; return *this; }
  vecteur& operator*=(T scalar) { x *= scalar; y *= scalar; return *this; }
  vecteur& operator/=(T scalar) { x /= scalar; y /= scalar; return *this; }

  //comparison operators
  bool operator==(vecteur<T> const& other) const { return x == other.x && y == other.y; }
  bool operator!=(vecteur<T> const& other) const { return x != other.x || y != other.y; }


  //operators with special effect
  //'inclusion' operator : change self so it is included into the box defined by dim
  vecteur& operator >>=(dimension<T> const& dim)
  {
    x = std::max( T(0), std::min( x, dim.GetWidth() ) );
    y = std::max( T(0), std::min( y, dim.GetHeight() ) );
    return *this;
  }

  //given a pitch, returns the offset corresponding to self
  int operator%(int pitch) const { return x + pitch * y; }  

};


//global scope swap
template <typename T>
inline void swap(vecteur<T>& left, vecteur<T>& right) 
{ 
  left.swap(right); 
}

//parallel min and max
template <typename T>
inline vecteur<T> min(vecteur<T> const& left, vecteur<T> const& right) { return vecteur<T>( std::min(left.x, right.x), std::min(left.y, right.y) ); }
template <typename T>
inline vecteur<T> max(vecteur<T> const& left, vecteur<T> const& right) { return vecteur<T>( std::max(left.x, right.x), std::max(left.y, right.y) ); }


template <typename T>
inline dimension<T>::dimension<T>(vecteur<T> const& vect) : x_( CheckSign(vect.x) ), y_( CheckSign(vect.y) ) { }

template <typename T>
inline dimension<T>& dimension<T>::operator+=(vecteur<T> const& vect)
{
  T x = CheckSign(x_ + vect.x);
  y_ = CheckSign(y_ + vect.y);
  x_ += x;
  return *this;
}



} //namespace avs

#endif //__AVS_VECTEUR_H__
