// Avisynth v3.0 alpha.  Copyright 2003 Ben Rudiak-Gould et al.
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
#include "resize.h"

//stl include
#include <math.h>    //for ceil, sin, fabs
#include <malloc.h>  //for _aligned_malloc


namespace avs { namespace filters {



class Resize::ResamplingPattern::Balancer
{ 

  const Filter& filter_;               //resize filter used
  double total, partial, x, step_;
  int minus, count;

  // fixed point scaler
  // Original value: 65536
  // 2 bits sacrificed because of 16 bit signed MMX multiplication
  static const int FPScale; //= 16384;


public:  //constructor

  Balancer(const Filter& filter, double step, int size)
    : filter_( filter )
    , step_( step )
    , count( size ) { }


public:

  void reset(double start)
  {
    total = filter_.total( start, step_, count );
    partial = 0;
    x = start;
    minus = 0;
  }

  
  int next() 
  {
    partial += filter_(x) / total;
    int plus = int( partial * FPScale + 0.5 );

    int coeff = plus - minus;

    minus = plus;
    x += step_;

    return coeff;
  }

};


Resize::ResamplingPattern::Straight::Straight(const Filter& filter, const SubRange& subrange, int width, int src_width)
  : ResamplingPattern(subrange, src_width)
{

  double length = subrange.upper - subrange.lower;

  double step = std::min( width / length, 1.0 );  
  double support = filter.support() / step;

  int aligned_width = (width + 15) & ~15;

  count_ = int( ceil(support * 2) );  
  pattern = new int[ width * (1 + count_) ];   //naturally provides int alignement

  int * ptr = pattern;


  // the following translates such that the image center remains fixed
  double pos = subrange.lower + (length - width) / (width * 2);

  Balancer balancer(filter, step, count_);

  for ( int i = aligned_width; i-- > 0; pos += length / width )
  {
    int end = std::min( int( pos + support ), src_width - 1 );
    
    int start = std::max( 0, end - count_ + 1 );

    *ptr++ = start;
        
    balancer.reset( (start - pos) * step );
    
    for ( int k = count_; k-- > 0; )
      *ptr++ = balancer.next();
  }

}


Resize::ResamplingPattern::MMXInterleave::MMXInterleave(const Filter& filter, const SubRange& subrange, int width, int src_width)
  : ResamplingPattern(subrange, src_width)
{

  double length = subrange.upper - subrange.lower;

  double step = std::min( width / length, 1.0 );  
  double support = filter.support() / step;

  int aligned_width = (width + 15) & ~15;

  int size = int( ceil(support * 2) );
  count_ = ( size >> 1 ) + 1;                  // number of loops in MMX code

  pattern = (int *) _aligned_malloc( aligned_width * (1 + count_) * 8, 8 );

  int * even = pattern;
  int * odd  = pattern + 1;


  // the following translates such that the image center remains fixed
  double pos = subrange.lower + (length - width) / (width * 2);
  
  Balancer balancer(filter, step, size);

  for ( int i = 0; i < aligned_width; ++i, pos += length / width )
  {
    int end = std::min( int(pos + support), src_width - 1 );
    int start = std::max( 0, end - size + 1 );

    int * & ptr = (i & 1) ? odd : even;
    *ptr = (start & -2) * 2;      
    ptr += 2;

    balancer.reset( (start - pos) * step );

    if ( start & 1 )
    {
      *ptr = balancer.next() << 16;
      ptr +=2;
    }
    for (int k = size >> 1; k-- > 0; )
    {
      int coeff = balancer.next();
      
      *ptr = (balancer.next() << 16) + (coeff & 0xFFFF);
      ptr += 2;      
    }
    if ( ! (start & 1) )  //if edge effect was not on beginning
    {
      //if size is odd: there is an extra coeff not handled by the for above
      *ptr = ( size & 1 ) ? balancer.next() & 0xFFFF
                           : 0;           //else we just set 0
      ptr += 2;
    }    

  }
}



double Resize::Filter::total(double start, double step, int count) const
{
  double result = 0;
  double x = start;

  for( int i = count; i-- > 0; x += step )  //count is supposed small enough to 
    result += operator()(x);

  return result;
}


double Resize::Filter::Point::operator ()(double x) const
{
  x = fabs(x);

  return ( x < 0.0 ) ? x : 0.0;  // Can somebody confirm this function?
}


double Resize::Filter::Triangle::operator ()(double x) const
{
  x = fabs(x);

  return ( x < 1.0 ) ? 1.0 - x : 0.0;
}


Resize::Filter::MitchellNetravali::MitchellNetravali(double b, double c)
{
  p0 = (   6. -  2. * b             ) / 6.;
  p2 = ( -18. + 12. * b  +  6. * c  ) / 6.;
  p3 = (  12. -  9. * b  -  6. * c  ) / 6.;
  q0 = (         8. * b  + 24. * c  ) / 6.;
  q1 = (      - 12. * b  - 48. * c  ) / 6.;
  q2 = (         6. * b  + 30. * c  ) / 6.;
  q3 = (      -       b  -  6. * c  ) / 6.;

}

double Resize::Filter::MitchellNetravali::operator ()(double x) const
{
  x = fabs(x);

  return (x < 1.0) ? (p0 + x * x * (p2 + x * p3))
                   : (x < 2.0) ? (q0 + x * (q1 + x * (q2 + x * q3)))
                               : 0.0;
}


double Resize::Filter::Lanczos3::operator ()(double x) const
{
  x = fabs(x);

  static const double pi = 3.14159265358979323846;
  static const double pi3 = pi / 3;

  return (x < 3.0) ? sinc(pi * x) * sinc(pi3 * x)
                   : 0.0;
}


double Resize::Filter::Lanczos3::sinc(double x) const
{
  return x != 0.0 ? sin(x) / x 
                  : 1.0;         //limit when x -> 0
}



}; }; //namespace avs::filters