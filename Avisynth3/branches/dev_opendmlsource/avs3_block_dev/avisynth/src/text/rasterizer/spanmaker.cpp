// Avisynth v3.0 alpha.  Copyright 2005-2006 David Pierre - Ben Rudiak-Gould et al.
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
#include "spanmaker.h"
#include "../../core/utility/round.h"

//stlport include
#include <cassert>


namespace avs { namespace text { namespace rasterizer {



void SpanMaker::StartPolygon(VecteurFP3 const& pt, bool clockWiseFill)
{
  assert(IsPolygonClosed());        //ensure previous polygon (if there is any) is closed

  startPt_ = lastPt_ = pt;          //update start and last pt
  clockWiseFill_ = clockWiseFill;   //update fill convention
}


void SpanMaker::LineTo(VecteurFP3 const& pt)
{
  long startY = utility::RoundingShift<3>(lastPt_.y.get());
  long endY   = utility::RoundingShift<3>(pt.y.get());

  if ( startY != endY )         //horizontal edges doesn't yield spans, so no work then
  {
    long dy = pt.y.get() - lastPt_.y.get();                             //not zero
    long long invSlope = ((pt.x.get() - lastPt_.x.get()) << 16) / dy;   //16 bits fractional part

    if ( dy < 0 )
      std::swap( startY, endY );

    //that's the x on the line at y = startY, with 16 bits fractional part
    long long x = (lastPt_.x.get() << 13) + ((( (startY << 3) - lastPt_.y.get() ) * invSlope) >> 3);

    for ( long y = startY; y < endY; ++y, x += invSlope )
      if ( dy > 0 && clockWiseFill_ )        //if going up and clockwise fill
        flipMap_[ y ].enter.push_back( static_cast<long>(utility::RoundingShift<16>(x)) );   //enter transition
      else
        flipMap_[ y ].exit.push_back( static_cast<long>(utility::RoundingShift<16>(x)) );    //else exit transition
  }

  lastPt_ = pt;          //update last point
}



} } } //namespace avs::text::rasterizer
