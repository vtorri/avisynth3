// Avisynth v3.0 alpha.  Copyright 2004 Ben Rudiak-Gould et al.
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


#ifndef __AVS_FILTERS_RESIZE_PATTERN_MAKER_H__
#define __AVS_FILTERS_RESIZE_PATTERN_MAKER_H__

//avisynth include
#include "../filter.h"
#include "../subrange.h"

//boost include
#include <boost/utility.hpp>


namespace avs { namespace filters { namespace resize { namespace pattern {



///////////////////////////////////////////////////////////////////////////////
//  Maker
//
//  helper class factorising pattern construction code
//
class Maker : private boost::noncopyable
{

  Filter const& filter_;
  double const scale;
  double const step;
  double const support;
  int const count_;
  int const max_end;

  double center;
  double total, partial, current;
  int minus;

public:  //constructor

  Maker(Filter const& filter, SubRange const& subrange, int size);


public: 
  
  //return number of significative coefficients it intends to produce
  int count() { return count_; }

  //produce an offset and get ready to produce coeffs
  int offset();

  //produce a coeff and get ready for the next
  int coeff();


private:  //implementation

  //ready self to produce coeffs at a new position
  void reset(double start);

};



} } } } //namespace avs::filters::resize::pattern

#endif //__AVS_FILTERS_RESIZE_PATTERN_MAKER_H__