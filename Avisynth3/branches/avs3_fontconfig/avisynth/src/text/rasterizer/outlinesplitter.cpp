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



//avisynth include
#include "outlinesplitter.h"

//stl include
#include <cmath>    //for sqrt


namespace avs { namespace text { namespace rasterizer {



void OutlineSplitter::BezierCurveTo(VecteurFP3 const& pt3, VecteurFP3 const& pt2, VecteurFP3 const& pt1)
{
  VecteurFP3 const& pt0 = spanMaker_.GetLastPt();

  VecteurFP3 A = pt3 - 3 * pt2 + 3 * pt1 -     pt0;
  VecteurFP3 B =       3 * pt2 - 6 * pt1 + 3 * pt0;
  VecteurFP3 C =                 3 * pt1 - 3 * pt0;
  VecteurFP3 D =                               pt0;   //a copy must be made, the last pt change during interpolation

  InterpolatePolynom(VecteurDb(A), VecteurDb(B), VecteurDb(C), D);

  LineTo(pt3);
}


void OutlineSplitter::BezierCurveTo(VecteurFP3 const& pt2, VecteurFP3 const& pt1)
{
  VecteurFP3 const& pt0 = spanMaker_.GetLastPt();

  VecteurFP3 B = pt2 - 2 * pt1 +     pt0;
  VecteurFP3 C =       2 * pt1 - 2 * pt0;
  VecteurFP3 D =                     pt0;

  InterpolatePolynom(VecteurDb(), VecteurDb(B), VecteurDb(C), D);

  LineTo(pt2);
}


namespace {

//small helper for InterpolatePolynom
double NormMajorant(vecteur<double> const& vect) { return std::fabs(vect.x) + std::fabs(vect.y); }

} //namespace anonymous


void OutlineSplitter::InterpolatePolynom(VecteurDb const& A, VecteurDb const& B, VecteurDb const& C, VecteurFP3 const& D)
{
  //we search a step d small enough so that our interpolating x(t) = A*t^3 + B*t^2 + C*t (A, B, C vectorials)
  //by line sections does not incur errors in the upcoming span creation
  //
  //ie for h in [0, d] we want to replace x(t + h) by x(t)*(1 - h/d) + x(t + d)*h/d
  //
  //| x(t+h) - x(t)*(1 - h/d) - x(t + d)*h/d |
  //  = |            x(t) +     h*x'(t) +     h^2/2*x''(t) +     h^3/6*x'''(t)
  //      -(1 - h/d)*x(t)
  //      -      h/d*x(t) - d*h/d*x'(t) + d^2/2*h/d*x''(t) + d^3/6*h/d*x'''(t) |
  //
  //  = | (h^2 + hd)/2*x''(t) + (h^3 + h*d^2)/6*x'''(t) |
  // <= d^2 * | 6*A*t + 2*B | + d^3/3 * | 6*A |
  //
  // <= 2 d^2 * max( | 3 A + B |, | B | ) + 2 d^3 * | A |
  //
  // <= d^2 * ( 2 max( | 3 A + B |, | B | ) + | A | )

  double M = 2 * std::max( NormMajorant(3 * A + B), NormMajorant(B) ) + NormMajorant(A);

  double step = (M < 1.0) ? 1.0
                          : (M < 4.0) ? 0.5
                                      : sqrt(1.0 / M);

  for ( double t = 0.0; t < 1.0; t += step )
    LineTo( D + VecteurFP3(((A * t + B ) * t + C) * t) );

}



} } } //namespace avs::text::rasterizer
