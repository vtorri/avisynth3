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


#ifndef __AVS_DIMENSION_H__
#define __AVS_DIMENSION_H__

//avisynth includes
#include "../roundup.h"
#include "../exception/baddimension.h"

//stl include
#include <assert.h>


namespace avs {


//class declaration
template <typename T> struct vecteur;



/////////////////////////////////////////////////////////////////////////////////////////
//  dimension<T>
//
//  helper template class to represent dimensions
//
template <typename T> class dimension
{

  T x_, y_;

  static T CheckSign(T val)
  {
    if ( val < T(0) )
      throw exception::BadDimension();
    return val;
  }


public:  //constructors

  dimension() : x_( 0 ), y_( 0 ) { }
  dimension(T x, T y) : x_( CheckSign(x) ), y_( CheckSign(y) ) { }
  dimension(dimension<T> const& other) : x_( other.x_ ), y_( other.y_ ) { }
  
  explicit dimension(vecteur<T> const& vect);


public:  //assignment

  dimension<T>& operator=(dimension<T> const& other) 
  { 
    x_ = other.x_; 
    y_ = other.y_; 
    return *this; 
  }

  void swap(dimension<T>& other) 
  {
    using std::swap;       //allows to find swap through arg dependent lookup

    swap(x_, other.x_);    //ie it may not be std::swap that will be used
    swap(y_, other.y_); 
  }


public:  //access

  T GetWidth() const { return x_; }
  T GetHeight() const { return y_; }

  void SetWidth(T width) { x_ = CheckSign(width); }
  void SetHeight(T height) { y_ = CheckSign(height); }


public:  //operators

  dimension<T>& operator+=(dimension<T> const& other)
  {
    x_ += other.x_;
    y_ += other.y_;
    return *this;
  }

  dimension<T>& operator+=(vecteur<T> const& vect);


  bool operator==(dimension<T> const& other) const { return x_ == other.x_ && y_ == other.y_; }
  bool operator!=(dimension<T> const& other) const { return x_ != other.x_ || y_ != other.y_; }


  //special effect operator: inclusion
  dimension<T>& operator >>=(dimension<T> const& other)
  {
    if ( x_ > other.x_ )
      x_ = other.x_;
    if ( y_ > other.y_ )
      y_ = other.y_;
    return *this;
  }


public:  //others

  bool empty() const { return x_ == 0 || y_ == 0; }

  template <long bps> dimension<T> Turn() const 
  { 
    assert(x_ % bps == 0); 
    return dimension<T>(y_ * bps, x_ / bps); 
  }

  template <long xFactor, long yFactor> dimension<T> Multiply() const { return dimension<T>(x_ * xFactor, y_ * yFactor); }
  template <long xFactor, long yFactor> dimension<T> Divide() const { return dimension<T>(x_ / xFactor, y_ / yFactor); }

  template <long xShift, long yShift> dimension<T> Shift() const { return dimension<T>(x_ + xShift, y_ + yShift); }

  template <long xRound, long yRound> dimension<T> Round() const { return dimension<T>( RoundUp<xRound>(x_), RoundUp<yRound>(y_) ); }

};


//global scope swap
template <typename T>
inline void swap(dimension<T>& left, dimension<T>& right) { left.swap(right); }

//global scope operator+
template <typename T>
inline dimension<T> operator+(dimension<T> const& left, dimension<T> const& right)
{
  dimension<T> result(left);
  result += right;
  return result;
}



} //namespace avs

#endif //__AVS_DIMENSION_H__
