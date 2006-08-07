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


#ifndef __AVS_FILTERS_SOURCE_AUDIODECOMPRESSOR_H__
#define __AVS_FILTERS_SOURCE_AUDIODECOMPRESSOR_H__

//avisynth includes
#include "../../../define.h"          //for AVS_NOVTABLE
#include "../../../core/forward.h"    //for BYTE


namespace avs { namespace filters { namespace source {



///////////////////////////////////////////////////////////////////////////////////////////
//  AudioDecompressor
//
//
//
class AVS_NOVTABLE AudioDecompressor
{

public:  //structors

  AudioDecompressor() { }
  virtual ~AudioDecompressor() { }


public:  //AudioDecompressor interface

  virtual long operator()(BYTE *& buffer, long long start, long count) const = 0;

};



} } } //namespace avs::filters::source

#endif //__AVS_FILTERS_SOURCE_AUDIODECOMPRESSOR_H__
