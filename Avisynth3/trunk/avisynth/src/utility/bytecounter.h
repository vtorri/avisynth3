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


#ifndef __AVS_BYTECOUNTER_H__
#define __AVS_BYTECOUNTER_H__

//avisynth includes
#include "../core/forward.h"         //for the WindowPtr typedef
#include "../core/window_ptr.h"      //for the definition

//stl include
#include <algorithm>                 //for std::copy


namespace avs {



////////////////////////////////////////////////////////////////////////////////////////////////
//  ByteCounter
//
//  helper class to count occurences of BYTE values
//
class ByteCounter
{

  long count_[256];


public:  //structors

  ByteCounter()
  {
    std::fill(count_, count_ + 256, 0L);
  }

  ByteCounter(ByteCounter const& other)
  {
    std::copy( other.count_, other.count_ + 256, count_ );
  }

  //generated destructor is fine


public:  //assignemnt

  ByteCounter& operator=(ByteCounter const& other)
  {
    std::copy( other.count_, other.count_ + 256, count_ );
  }

  void swap(ByteCounter& other)
  {
    std::swap_ranges( other.count_, other.count_ + 256, count_ );
  }


public:  //read access

  long operator[](int i) const { assert(i >= 0 && i < 256); return count_[i]; }
  long const * data() const { return count_; }


public:  //interface

  template <int step>
  void Count(WindowPtr const& wp)
  {
    assert( wp.width % step == 0 );

    BYTE * ptr = wp.ptr;

    for ( int y = wp.height; y-- > 0; ptr += wp.padValue() )
      for ( int x = wp.width / step; x-- > 0; ptr += step )
        ++count_[*ptr];
  }

  int GetMin() const;       //fetchs smallest index yielding a non-zero count
  int GetMax() const;       //fetchs biggest index yielding a non-zero count

  int GetLooseMin(long limit) const;
  int GetLooseMax(long limit) const;

  long GetTotal() const;               //fetchs total count for all values
  long long GetWeightedTotal() const;  //fetchs total of count[i] * i

};



} //namespace avs

#endif //__AVS_BYTECOUNTER_H__
