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

//avisynth include
#include "exception/baddimension.h"


namespace avs {


//class declaration
struct Vecteur;



/////////////////////////////////////////////////////////////////////////////////////////
//  Dimension
//
//  helper class to represent dimensions
//
class Dimension
{

  int x_, y_;

  static int CheckSign(int val)
  {
    if ( val < 0 )
      throw BadDimension();
    return val;
  }

public:  //constructors

  Dimension() : x_( 0 ), y_( 0 ){ }
  Dimension(int x, int y) : x_( CheckSign(x) ), y_( CheckSign(y) ) { }
  Dimension(Dimension const& other) : x_( other.x_ ), y_( other.y_ ) { }
  
  explicit Dimension(Vecteur const& vect);


public:  //assignment

  Dimension& operator=(Dimension const& other) { x_ = other.x_; y_ = other.y_; return *this; }

  void swap(Dimension& other) { std::swap(x_, other.x_); std::swap(y_, other.y_); }


public:  //access

  int GetWidth() const { return x_; }
  int GetHeight() const { return y_; }

  void SetWidth(int width) { x_ = CheckSign(width); }
  void SetHeight(int height) { y_ = CheckSign(height); }


public:  //operators

  Dimension& operator+=(Dimension const& other)
  {
    x_ += other.x_;
    y_ += other.y_;
    return *this;
  }

  Dimension& operator+=(const Vecteur vect);


  bool operator==(Dimension const& other) const { return x_ == other.x_ && y_ == other.y_; }
  bool operator!=(Dimension const& other) const { return x_ != other.x_ || y_ != other.y_; }
  
  //special effect operator: inclusion
  Dimension& operator >>=(Dimension const& other)
  {
    if ( x_ > other.x_ )
      x_ = other.x_;
    if ( y_ > other.y_ )
      y_ = other.y_;
    return *this;
  }


public:  //others

  bool empty() const { return x_ == 0 || y_ == 0; }

  template <int bps> Dimension Turn() const { return Dimension(y_ * bps, x_ / bps); }

  template <int xFactor, int yFactor> Dimension Multiply() const { return Dimension(x_ * xFactor, y_ * yFactor); }
  template <int xFactor, int yFactor> Dimension Divide() const { return Dimension(x_ / xFactor, y_ / yFactor); }

  template <int xShift, int yShift> Dimension Shift() const { return Dimension(x_ + xShift, y_ + yShift); }

};//Dimension



} //namespace avs

#endif //__AVS_DIMENSION_H__