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


//avisynth include
#include "maker.h"

//stl include
#include <math.h>    //for ceil


namespace avs { namespace filters { namespace resize { namespace pattern {


Maker::Maker(Filter const& filter, SubRange const& subrange, int size)
  : filter_( filter )    
  , scale( subrange.length() / size )  
  , step( std::min(size / subrange.length(), 1.0) )
  , support( filter.support() / step )
  , count_( (int)ceil(support * 2) )
  , max_end( subrange.whole() - 1 )
{
  center = subrange.lower() + (scale - 1.0) / 2;
}


int Maker::offset()
{
  int end = std::min( int( center + support ), max_end );
  int start = std::max( 0, end - count_ + 1 );

  reset( (start - center) * step );     //ready self to produce coeffs for this offset
  center += scale;                      //update center for next offset

  return start;                         //return offset
}

int Maker::coeff()
{
  partial += filter_(current);
  int plus = int( partial * 16384 / total + 0.5 );

  int coeff_ = plus - minus;            //calc coeff

  minus = plus;                         //update for next coeff
  current += step;                      //idem

  return coeff_;
}


void Maker::reset(double start)
{
  total = 0;
  partial = 0;
  current = start;
  minus = 0;

  for( int i = count_; i-- > 0; start += step )
    total += filter_(start);
}





} } } } //namespace avs::filters::resize::pattern