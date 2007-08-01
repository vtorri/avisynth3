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


#ifndef __AVS_FILTERS_RESIZE_PATTERN_INTERLEAVED2X2_H__
#define __AVS_FILTERS_RESIZE_PATTERN_INTERLEAVED2X2_H__

//avisynth includes
#include "base.h"
#include "../forward.h"


namespace avs { namespace filters { namespace resize { namespace pattern {


//template declaration so I can make the specialisation below
template <int pixelCount, int coeffCount> class interleaved;



//////////////////////////////////////////////////////////////////////////////////////////////
//  interleaved<2, 2>
//
//  The pattern datas for 2 pixels are interleaved,
//  with coefficients coded as shorts and grouped by 2.
//  Besides the offsets are guaranteed even (done by inserting coeffs to compensate)
//
//  For that:
//     size (number of pixels) is rounded even up (may introduce dummy pixel data)
//     count is padded to the next even value (so we have room for the compensation)    
//
//  It produces the following layout:
//    off0 | off1 | coeff block | coeff block ... coeff block | off0 | off1 | coeff block ...
//                    <----      count / 2 times      ---->
//
//  Where  | coeff block |  =  | c0a|c0b | c1a|c1b |   2 short coeffs for each pixel
//
//  The pattern is quaranteed 8 bytes aligned, making each offset int aligned
//  and each coeff block 8 bytes aligned
//
template <> class interleaved<2, 2> : public Base
{

public:  //structors

  interleaved<2, 2>(PEnvironment const& env, Filter const& filter, SubRange const& subrange, int size);

  //generated copy constructor and destructor are fine

};


} } } } //namespace avs::filters::resize::pattern

#endif //__AVS_FILTERS_RESIZE_PATTERN_INTERLEAVED2X2_H__
