// Avisynth v3.0 alpha.  Copyright 2005 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_FIXEDPOINT_H__
#define __AVS_FIXEDPOINT_H__


namespace avs {



/////////////////////////////////////////////////////////////////////////////////////////////
//  fixed_point<fractionPart>
//
//  for fixed point arithmetic
//
template <int fractionPart> 
class fixed_point
{

  typedef long BaseType;
  typedef fixed_point<fractionPart> FixedPointType;

  BaseType value_;


public:  //structors

  explicit fixed_point(BaseType value)
    : value_( value ) { }

  //generated copy constructor and destructor are fine


public:  //assignment

  //generated operator= is fine

  void swap(FixedPointType& other) { std::swap(value_, other.value_); }


public:  //comparison operators

  bool operator==(FixedPointType const& other) const { return value_ == other.value_; }
  bool operator!=(FixedPointType const& other) const { return value_ != other.value_; }


public:  //operators

  FixedPointType& operator+=(FixedPointType const& other) { value_ += other.value_; return *this; }
  FixedPointType& operator-=(FixedPointType const& other) { value_ -= other.value_; return *this; }


public:  //access

  BaseType get() const { return value_; }

};


//global scope swap
template <int fp>
inline void swap(fixed_point<fp>& left, fixed_point<fp>& right) { left.swap(right) }



} //namespace avs

#endif //__AVS_FIXEDPOINT_H__
