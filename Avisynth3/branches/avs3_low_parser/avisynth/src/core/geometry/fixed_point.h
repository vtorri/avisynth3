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

//boost include
#include <boost/operators.hpp>


namespace avs {



/////////////////////////////////////////////////////////////////////////////////////////////
//  fixed_point<fractionPart>
//
//  for fixed point arithmetic
//
template <int fractionPart> 
class fixed_point 
    : boost::addable<fixed_point<fractionPart>                 //defines +
    , boost::subtractable<fixed_point<fractionPart>            //defines -
    , boost::right_shiftable<fixed_point<fractionPart>, int    //defines >>
    , boost::left_shiftable<fixed_point<fractionPart>, int     //defines <<
      > > > >
{

  typedef long BaseType;
  typedef fixed_point<fractionPart> FixedPointType;

  BaseType value_;


public:  //structors

  fixed_point(BaseType value)
    : value_( value << fractionPart ) { }

  template <typename T>
  explicit fixed_point(T value)
    : value_( static_cast<BaseType>(value * (1 << fractionPart)) ) { }

  //generated copy constructor and destructor are fine


public:  //wrapping (non-shifting) constructor and helper method

  struct wrap { };

  fixed_point(BaseType value, wrap dummy)
    : value_( value ) { }

  static FixedPointType Wrap(BaseType value) { return FixedPointType(value, wrap()); }


public:  //assignment

  //generated operator= is fine

  void swap(FixedPointType& other) { std::swap(value_, other.value_); }


public:  //comparison operators

  bool operator==(FixedPointType const& other) const { return value_ == other.value_; }
  bool operator!=(FixedPointType const& other) const { return value_ != other.value_; }

  bool operator<(FixedPointType const& other) const { return value_ < other.value_; }
  bool operator>(FixedPointType const& other) const { return value_ > other.value_; }

  bool operator<=(FixedPointType const& other) const { return value_ <= other.value_; }
  bool operator>=(FixedPointType const& other) const { return value_ >= other.value_; }


public:  //operators

  FixedPointType operator-() const { return Wrap(-value_); }

  FixedPointType& operator+=(FixedPointType const& other) { value_ += other.value_; return *this; }
  FixedPointType& operator-=(FixedPointType const& other) { value_ -= other.value_; return *this; }

  FixedPointType& operator*=(BaseType scalar) { value_ *= scalar; return *this; }
  FixedPointType& operator/=(BaseType scalar) { value_ /= scalar; return *this; }

  FixedPointType& operator<<=(int shift) { value_ <<= shift; return *this; }
  FixedPointType& operator>>=(int shift) { value_ >>= shift; return *this; }


public:  //conversion operators

  operator float() const { return static_cast<float>(value_) / (1 << fractionPart); }
  operator double() const { return static_cast<double>(value_) / (1 << fractionPart); }


public:  //access

  BaseType get() const { return value_; }

  BaseType GetRoundedUp() const { return utility::RoundingUpShift<fractionPart>(value_); }
  BaseType GetRounded() const { return utility::RoundingShift<fractionPart>(value_); }
  BaseType GetRoundedDown() const { return utility::RoundingDownShift<fractionPart>(value_); }

};


//global scope swap
template <int fp>
inline void swap(fixed_point<fp>& left, fixed_point<fp>& right) { left.swap(right); }



} //namespace avs

#endif //__AVS_FIXEDPOINT_H__
