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


//avisynth includes
#include "spanmap.h"
#include "spanmaker.h"

//stl include
#include <cmath>

//assert include
#include "assert.h"


namespace avs { namespace text { namespace rasterizer {



SpanMap::SpanMap(SpanMaker const& maker)
{
  assert( maker.IsPolygonClosed());

  map_.insert(maker.begin(), maker.end());   //implicitly converts values from one map into values of the other :)
}



SpanMap SpanMap::Thicken(float radius) const
{
  SpanMap result;

  long r = static_cast<long>( std::floor(radius) );

  for ( long dy = -r; dy <= r; ++dy )
  {
    long dx = static_cast<long>( std::sqrt( radius * radius - dy*dy ) );

    for ( Map::const_iterator it = map_.begin(); it != map_.end(); ++it )
      result.map_[ it->first + dy ].MergeThickened( it->second, dx );
  }

  return result;
}


} } } //namespace avs::text::rasterizer
