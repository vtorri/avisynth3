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
#include "../core/vecteur.h"
#include "../core/dimension.h"

//boost include
#include <boost/operators.hpp>


namespace avs {



class Box
     : boost::addable<Box             //defines Box + Box
     , boost::addable2<Box, Vecteur>  //defines Box + Vecteur
     >
{

  Vecteur pos_;
  Dimension size_;


public:  //structors

  Box() { }
  Box(Vecteur const& pos, Dimension const& size) : pos_( pos ), size_( size ) { }
  
  //generated destructor and copy constructor are fine


public:  //assignment

  //generated operator= is fine

  void swap(Box& other) { pos_.swap(other.pos_), size_.swap(other.size_);


public:  //operators

  Box& operator+=(Vecteur const& vect)
  {
    pos_ += vect;
    return *this;
  }

  Box& operator+=(Box const& other)
  {
    Vecteur position = min(pos_, other.pos_);                                     //get top left corner of the box
    size_ = Dimension( max(pos_ + size_, other.pos_ + other.size_) - position );  //calculate bottom right
    pos_ = position;                                                              //update pos_
    return *this;
  }

  bool operator==(Box const& other) const { return size_ == other.size_ && position_ == other.position_; }
  bool operator!=(Box const& other) const { return size_ != other.size_ || position_ != other.position_; }

};


//global scope swap
inline void swap(Box& left, Box& right) { left.swap(right); }


} //namespace avs

#endif //__AVS_BOX_H__
