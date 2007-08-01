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


//avisynth includes
#include "point.h"
#include "lanczos3.h"
#include "triangle.h"
#include "mitchellnetravali.h"

//boost include
#include <boost/math/special_functions/sinc.hpp>   //for sinc

//stl include
#include <cmath>    //for ceil, fabs


namespace avs { namespace filters { namespace resize { namespace filter {




double Point::operator ()(double x) const
{
  return 1.0;
}


double Triangle::operator ()(double x) const
{
  x = fabs(x);

  return ( x < 1.0 ) ? 1.0 - x : 0.0;
}


MitchellNetravali::MitchellNetravali(double b, double c)
{
  p0 = (   6. -  2. * b             ) / 6.;
  p2 = ( -18. + 12. * b  +  6. * c  ) / 6.;
  p3 = (  12. -  9. * b  -  6. * c  ) / 6.;
  q0 = (         8. * b  + 24. * c  ) / 6.;
  q1 = (      - 12. * b  - 48. * c  ) / 6.;
  q2 = (         6. * b  + 30. * c  ) / 6.;
  q3 = (      -       b  -  6. * c  ) / 6.;

}

double MitchellNetravali::operator ()(double x) const
{
  x = fabs(x);

  return (x < 1.0) ? (p0 + x * x * (p2 + x * p3))
                   : (x < 2.0) ? (q0 + x * (q1 + x * (q2 + x * q3)))
                               : 0.0;
}


double Lanczos3::operator ()(double x) const
{
  x = fabs(x);

  static double const pi = 3.14159265358979323846;
  static double const pi3 = pi / 3;

  return (x < 3.0) ? boost::math::sinc_pi(pi * x) * boost::math::sinc_pi(pi3 * x)
                   : 0.0;
}




} } } } //namespace avs::filters::resize::filter
