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
#include "levels.h"
#include "lumamap.h"
#include "../../../core/utility/saturate.h"

//stl include
#include <cmath>   //for floor, pow


namespace avs { namespace filters { namespace coloryuv {



LumaMap::LumaMap(Levels const& levels, float gamma, int (* adjust)(int), bool coring)
{
    
  for ( int i = 0; i < 256; ++i )
  {
    float value = static_cast<float>(adjust(i));
      
    value += value * levels.gain / 256 + (value - 128) * levels.contrast / 256 + levels.brightness;
      
    if ( gamma != 0 && value > 0 )
      value = 256 * std::pow( value / 256, 1 / std::max(0.01f, (gamma + 256) / 256) );
    
    int val = static_cast<int>( std::floor(value + 0.5f) );
            
    if ( coring )
      if ( val > 235 )
        val = 235;
      else
        if ( val < 16 )
          val = 16;

    operator[](i) = saturate<BYTE>(val);
  }

}



} } } //namespace avs::filters::coloryuv
