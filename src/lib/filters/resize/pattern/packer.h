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


#ifndef __AVS_FILTERS_RESIZE_PATTERN_PACKER_H__
#define __AVS_FILTERS_RESIZE_PATTERN_PACKER_H__

//boost include
#include <boost/utility.hpp>   //for noncopyable


namespace avs { namespace filters { namespace resize { namespace pattern {



///////////////////////////////////////////////////////////////////////////////////////
//  packer<groupCoeffBy, packerCount>
//
//  helper class to pack offset and coeffs into a pattern
//
//
template <int groupCoeffPairBy, int packerCount>
class packer : private boost::noncopyable
{

  short coeffs[groupCoeffPairBy * 2];
  int * ptr_;                 //pattern ptr
  int toCoeffs;               //jump between the offset part and the coeffs part
  int i;                      //current position into the coeffs array


public:  //structors

  packer<groupCoeffPairBy, packerCount>(int no, int * ptr)
    : ptr_( ptr )
    , toCoeffs( packerCount + no * (groupCoeffPairBy - 1)  )
    , i( 0 ) { }

  //generated destructor is fine


public:  //packer methods

  //pack an offset and get ready to pack coeffs
  void offset(int value)
  {
    *ptr_ = value;
    ptr_ += toCoeffs;
  }

  void coeff(int value)
  {
    coeffs[i++] = static_cast<short>(value);

    if ( i == groupCoeffPairBy * 2 )          //we have enough coeffs to pack them
    {

      for( int k = 0; k < groupCoeffPairBy; ++k)
        ptr_[k] = ((int *)coeffs)[k];

      i = 0;                                  //reset i for next ones
      ptr_ += groupCoeffPairBy * packerCount; //move to next position

    }
  }

};




} } } } //namespace avs::filters::resize::pattern

#endif //__AVS_FILTERS_RESIZE_PATTERN_PACKER_H__
