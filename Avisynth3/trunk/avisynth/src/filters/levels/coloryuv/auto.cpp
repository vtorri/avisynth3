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
#include "auto.h"
#include "../../../core/videoframe.h"
#include "../../../utility/bytecounter.h"
#include "../../../core/utility/saturate.h"


namespace avs { namespace filters { namespace coloryuv {



Auto::Auto(PClip const& child, Levels const& Y, Levels const& U, Levels const& V, float gamma, Mode mode, bool coring, bool autoWhite, bool autoGain)
  : ColorYUV( child )
  , Y_( Y )
  , U_( U )
  , V_( V )
  , gamma_( gamma )
  , coring_( coring )
  , autoWhite_( autoWhite )
  , autoGain_( autoGain ) { }


PClip Auto::clone(PClip const& child) const
{
  return PClip( static_cast<Clip *>(new Auto(child, Y_, U_, V_, gamma_, mode_, coring_, autoWhite_, autoGain_)) );
}


CPVideoFrame Auto::MakeFrame(PVideoFrame const& source) const
{
  ByteCounter y, u, v;

  Count(*source, y, u, v);
  
  long yPixelCount = y.GetTotal();
  long yLooseLimit = yPixelCount >> 8;

  if ( autoWhite_ )
  {
    U_.contrast = 127.f - static_cast<float>(u.GetWeightedTotal()) / u.GetTotal();
    V_.contrast = 127.f - static_cast<float>(v.GetWeightedTotal()) / v.GetTotal();
  }

  if ( autoGain_ )
  {
    int min = std::max(16, y.GetLooseMin(yLooseLimit));
    int max = std::min(y.GetLooseMax(yLooseLimit), 236);    //don't scale above luma range 

    if ( min != max )
    {
      float scale = 220.f / (max - min);

      Y_.gain = 256.f * scale - 256.f;
      Y_.contrast = 16.f - scale * min;
    }
  }

  /*      
  

    MakeGammaLUT();*/


  return source;
}


} } } //namespace avs::filters::coloryuv
