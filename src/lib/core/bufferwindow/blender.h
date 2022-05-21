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


#ifndef __AVS_BW_BLENDER_H__
#define __AVS_BW_BLENDER_H__

//avisynth include
#include "../bufferwindow.h"


namespace avs { namespace bw {



/////////////////////////////////////////////////////////////////////////////////////////////
//  Blender<bps>
//
//  functor to  blend one BufferWindow into another
//
//  since there are no default implementation, this one declares nothing
//  see the two specialisations below
//
template <int bps> class Blender { };



/////////////////////////////////////////////////////////////////////////////////////////////
//  Blender<1>
//
//  Do the blending considering the data from each buffer is one byte per sample
//
template <> class Blender<1>
{

  int weight_;
  bool noBlend_;         //true if the blend is a no op
  bool fullBlend_;       //true if the blend is a copy from blendFrom to blendIn


public:  //structors

  Blender<1>(float factor);

  //generated copy constructor and destructor are fine


public:

  void operator()(BufferWindow& blendIn, BufferWindow const& blendFrom) const;

};


/////////////////////////////////////////////////////////////////////////////////////////////
//  Blender<2>
//
//  Do the blending considering the data from each buffer is two bytes per sample
//
//  TODO: code me
//
template <> class Blender<2>
{

public:  //structors

  Blender<2>(float /*factor*/) { }

  //generated copy constructor and destructor are fine


public:

  void operator()(BufferWindow& /*blendIn*/, BufferWindow const& /*blendFrom*/) const { }

};



} } //namespace avs::bw

#endif //__AVS_BW_BLENDER_H__
