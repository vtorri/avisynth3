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


#ifndef __AVS_BYTEMAP_H__
#define __AVS_BYTEMAP_H__

//avisynth includes
#include "../core/forward.h"           //for WindowPtr typedef
#include "../core/window_ptr.h"        //for its definition

//stl include
#include <algorithm>                   //for std::copy

//assert include
#include <assert.h>


namespace avs {



//////////////////////////////////////////////////////////////////////////////////////////
//  ByteMap
//
//  Maps byte values into processed (byte) values
//  This class is meant to be used as a base class, subclass constructor doing map init 
//  
class ByteMap
{

  unsigned char map_[256];


protected:  //structors

  ByteMap() { }

  ByteMap(ByteMap const& other)
  {
    std::copy( other.map_, other.map_ + 256, map_ );
  }

  //generated destructor is fine


protected:  //assignment

  ByteMap& operator=(ByteMap const& other)
  {
    std::copy( other.map_, other.map_ + 256, map_ );
    return *this;
  }

  void swap(ByteMap& other)
  {
    std::swap_ranges( other.map_, other.map_ + 256, map_ );
  }


public:  //read access

  unsigned char const operator[](int i) const { assert(i >= 0 && i < 256); return map_[i]; }
  unsigned char const * data() const { return map_; }


protected:  //write access

  unsigned char * data() { return map_; }
  unsigned char & operator[](int i) { return map_[i]; }


public:  //use method

  //use the lookup table on all the passed data
  template <int step>
  void ApplyTo(WindowPtr const& wp) const
  {
    assert( wp.width % step == 0 );

    BYTE * ptr = wp.ptr;

    for ( int y = wp.height; y-- > 0; ptr += wp.padValue() )
      for ( int x = wp.width / step; x-- > 0; ptr += step )
        *ptr = map_[ *ptr ];
  }

};



} //namespace avs 

#endif //__AVS_BYTEMAP_H__
