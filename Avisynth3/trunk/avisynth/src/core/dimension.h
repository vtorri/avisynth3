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


#ifndef __AVS_DIMENSION_H__
#define __AVS_DIMENSION_H__


//stl include
#include <string>  //needed or invalid_argument won't work


namespace avs {


//class declaration
struct Vecteur;



//////////////////////////////////////////////////////////////////////////////////
//  bad_dimension
//
//  exception class for bad Dimensions (negative width or height)
//
class bad_dimension : public std::invalid_argument
{

public:  
  bad_dimension() : std::invalid_argument("") { }


  virtual const char * what() const { return "Bad Dimension"; }

};


/////////////////////////////////////////////////////////////////////////////////////////
//  Dimension
//
//  helper class to represent dimensions
//
class Dimension
{

  int x, y;

  static int CheckSign(int val)
  {
    if (val < 0)
      throw bad_dimension();
    return val;
  }

public:  //constructors

  Dimension() : x( 0 ), y( 0 ){ }
  Dimension(int _x, int _y) : x( CheckSign(_x) ), y( CheckSign(_y) ) { }
  Dimension(Dimension const& other) : x( other.x), y( other.y ) { }
  
  explicit Dimension(Vecteur const& vect);


public:  //assignment

  Dimension& operator=(Dimension const& other) { x = other.x; y = other.y; return *this; }

  void swap(Dimension& other) { std::swap(x, other.x); std::swap(y, other.y); }


public:  //access

  int GetWidth() const { return x; }
  int GetHeight() const { return y; }

  void SetWidth(int width) { x = CheckSign(width); }
  void SetHeight(int height) { y = CheckSign(height); }


public:  //operators

  Dimension& operator+=(Dimension const& other)
  {
    x += other.x;
    y += other.y;
    return *this;
  }

  Dimension& operator+=(const Vecteur vect);

  bool operator==(Dimension const& other) const { return x == other.x && y == other.y; }
  bool operator!=(Dimension const& other) const { return x != other.x || y != other.y; }
  
  bool empty() const { return x == 0 || y == 0; }

};//Dimension



} //namespace avs

#endif //__AVS_DIMENSION_H__