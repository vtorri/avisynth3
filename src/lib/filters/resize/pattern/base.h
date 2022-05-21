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


#ifndef __AVS_FILTERS_RESIZE_PATTERN_BASE_H__
#define __AVS_FILTERS_RESIZE_PATTERN_BASE_H__

//avisynth include
#include "../../../core/ownedblock.h"


namespace avs { namespace filters { namespace resize { namespace pattern {



///////////////////////////////////////////////////////////////////////////////////////
//  Base
//
//  base class of pattern classes
//
class Base
{

  int count_;             //that is always the coeff count in the pattern, not the number of loops expected
  OwnedBlock pattern_;    //underlying OwnedBlock


protected:  //structors

  Base(PEnvironment const& env)
    : pattern_( env, 0, false ) { }

  Base(Base const& other)
    : count_( other.count_ )
    , pattern_( other.pattern_ ) { }

  //generated destructor is fine


protected:  //assignment

  void operator=(Base const& other)
  {
    count_ = other.count_;
    pattern_ = other.pattern_;
  }

  void swap(Base& other)
  {
    std::swap(count_, other.count_);
    pattern_.swap(other.pattern_);
  }


public:  //access

  //gets pattern data
  int const * Get() const { return reinterpret_cast<int *>(pattern_.Get()); }

  //always return the coeffs count per pixel, not the number of loops expected
  int Count() const { return count_; }


protected:  //at the disposition of subclasses constructor

  void Init(int count, int size)
  {
    count_ = count;
    pattern_.Reset( size << 2, block::OwnedCreator(pattern_, false) );
  }

  int * Get() { return reinterpret_cast<int *>(pattern_.Get()); }

};



} } } } //namespace avs::filters::resize::pattern

#endif //__AVS_FILTERS_RESIZE_PATTERN_BASE_H__
