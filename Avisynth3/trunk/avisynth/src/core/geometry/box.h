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
//
template <typename T>
class box
     : boost::andable<box<T>                 //defines Box & Box      (intersection)
     , boost::orable<box<T>                  //defines Box | Box      (union)
     , boost::addable2<box<T>, vecteur<T> >  //defines Box + Vecteur  (translation)
     > >
{

  typedef box<T> BoxType;
  typedef vecteur<T> VecteurType;
  typedef dimension<T> DimensionType;

  VecteurType pos_;
  DimensionType dim_;


public:  //structors

  box() { }
  box(VecteurType const& pos, DimensionType const& dim) : pos_( pos ), dim_( dim ) { }
  box(VecteurType const& topLeft, VecteurType const& bottomRight) : pos_( topLeft ), dim_( bottomRight - topLeft ) { }

  //generated destructor and copy constructor are fine


public:  //assignment

  //generated operator= is fine

  void swap(BoxType& other) { pos_.swap(other.pos_); dim_.swap(other.dim_); }


public:  //read access

  VecteurType const& GetPosition() const { return pos_; }
  DimensionType const& GetDimension() const { return dim_; }


public:  //comparison operators

  bool operator==(BoxType const& other) const { return dim_ == other.dim_ && pos_ == other.pos_; }
  bool operator!=(BoxType const& other) const { return dim_ != other.dim_ || pos_ != other.pos_; }


public:  //operators

  BoxType& operator&=(Box const& other)
  {
    VecteurType position = pmax(pos_, other.pos_);                                  //get top left corner of the box
    dim_ = DimensionType( pmin(pos_ + dim_, other.pos_ + other.dim_) - position );  //calculate bottom right
    pos_ = position;                                                                //update pos_
    return *this;
  }

  BoxType& operator|=(Box const& other)
  {
    VecteurType position = pmin(pos_, other.pos_);                                  //get top left corner of the box
    dim_ = DimensionType( pmax(pos_ + dim_, other.pos_ + other.dim_) - position );  //calculate bottom right
    pos_ = position;                                                                //update pos_
    return *this;
  }

  BoxType& operator+=(VecteurType const& vect)
  {
    pos_ += vect;
    return *this;
  }

};


//global scope swap
template <typename T>
inline void swap(box<T>& left, box<T>& right) { left.swap(right); }


} //namespace avs

#endif //__AVS_BOX_H__
