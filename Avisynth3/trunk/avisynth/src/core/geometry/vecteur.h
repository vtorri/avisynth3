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


namespace avs {



////////////////////////////////////////////////////////////////////////////////////////////
//  Vecteur
//
//
//
struct Vecteur
     : boost::addable<Vecteur                      //defines Vecteur + Vecteur
     , boost::subtractable<Vecteur                 //defines Vecteur - Vecteur
     , boost::right_shiftable<Vecteur, Dimension>  //defines Vecteur >> Dimension (inclusion)
       > >
{

  int x, y;   

  //constructors
  Vecteur() : x( 0 ), y( 0 ) { }
  Vecteur(int _x, int _y) : x( _x ), y( _y ) { }
  Vecteur(Vecteur const& other) : x( other.x ), y( other.y ) { }
  
  Vecteur(Dimension const& dim)  : x( dim.GetWidth() ), y( dim.GetHeight() ) { }

  //assignment
  Vecteur& operator=(Vecteur const& other) { x = other.x; y = other.y; return *this; }
  void swap(Vecteur& other) { std::swap(x, other.x); std::swap(y, other.y); }

  //classic vector operators
  Vecteur operator-() const { return Vecteur(-x, -y); }
  Vecteur& operator+=(Vecteur const& other) { x += other.x; y += other.y; return *this; }
  Vecteur& operator-=(Vecteur const& other) { x -= other.x; y -= other.y; return *this; }
  Vecteur& operator*=(int scalar) { x *= scalar; y *= scalar; return *this; }
  Vecteur& operator/=(int scalar) { x /= scalar; y /= scalar; return *this; }


  //operators with special effect
  //'inclusion' operator : change self so it is included into the box defined by dim
  Vecteur& operator >>=(Dimension const& dim)
  {
    x = std::max(0, std::min( x, dim.GetWidth() ));
    y = std::max(0, std::min( y, dim.GetHeight() ));
    return *this;
  }

  //given a pitch, returns the offset corresponding to self
  int operator%(int pitch) const { return x + pitch * y; }  

};


//global scope swap
inline void swap(Vecteur& left, Vecteur& right) { left.swap(right); }

//parallel min and max
inline Vecteur min(Vecteur const& left, Vecteur const& right) { return Vecteur( std::min(left.x, right.x), std::min(left.y, right.y) ); }
inline Vecteur max(Vecteur const& left, Vecteur const& right) { return Vecteur( std::max(left.x, right.x), std::max(left.y, right.y) ); }


inline Dimension::Dimension(Vecteur const& vect) : x_( CheckSign(vect.x) ), y_( CheckSign(vect.y) ) { }

inline Dimension& Dimension::operator+=(Vecteur const& vect)
{
  CheckSign(x_ + vect.x);
  y_ = CheckSign(y_ + vect.y);
  x_ += vect.x;
  return *this;
}



} //namespace avs

#endif //__AVS_VECTEUR_H__
