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


#ifndef __AVS_BOX_H__
#define __AVS_BOX_H__

//avisynth include
#include "vecteur.h"
#include "dimension.h"

//boost include
#include <boost/operators.hpp>


namespace avs {



///////////////////////////////////////////////////////////////////////////////////////////////////////
//  Box
//
//
class Box
     : boost::addable<Box             //defines Box + Box
     , boost::addable2<Box, Vecteur>  //defines Box + Vecteur
     >
{

  Vecteur pos_;
  Dimension dim_;


public:  //structors

  Box() { }
  Box(Vecteur const& pos, Dimension const& dim) : pos_( pos ), dim_( dim ) { }
  
  //generated destructor and copy constructor are fine


public:  //assignment

  //generated operator= is fine

  void swap(Box& other) { pos_.swap(other.pos_); dim_.swap(other.dim_); }


public:  //read access

  Vecteur const& GetPosition() const { return pos_; }
  Dimension const& GetDimension() const { return dim_; }


public:  //operators

  Box& operator+=(Vecteur const& vect)
  {
    pos_ += vect;
    return *this;
  }

  Box& operator+=(Box const& other)
  {
    Vecteur position = min(pos_, other.pos_);                                     //get top left corner of the box
    dim_ = Dimension( max(pos_ + dim_, other.pos_ + other.dim_) - position );     //calculate bottom right
    pos_ = position;                                                              //update pos_
    return *this;
  }

  bool operator==(Box const& other) const { return dim_ == other.dim_ && pos_ == other.pos_; }
  bool operator!=(Box const& other) const { return dim_ != other.dim_ || pos_ != other.pos_; }

};


//global scope swap
inline void swap(Box& left, Box& right) { left.swap(right); }


} //namespace avs

#endif //__AVS_BOX_H__
