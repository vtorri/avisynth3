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


#ifndef __AVS_FILTERS_RESIZE_PATTERN_SIMPLE_H__
#define __AVS_FILTERS_RESIZE_PATTERN_SIMPLE_H__

//avisynth include
#include "base.h"
#include "../filter.h"
#include "../subrange.h"


namespace avs { namespace filters { namespace resize { namespace pattern {



////////////////////////////////////////////////////////////////////////////////////
//  Simple
//
//  simple resampling pattern, built as follow:
//  For each pixel : one offset and count coeffs, all ints
//     offs | coef | coef ... coef | coef | offs | coef | coef ...            
//             <----  count times  ---->
//
class Simple : public Base
{

public:  //constructor

  Simple(PEnvironment const& env, Filter const& filter, SubRange const& subrange, int size);

};




} } } } //namespace avs::filters::resize::pattern

#endif //__AVS_FILTERS_RESIZE_PATTERN_SIMPLE_H__