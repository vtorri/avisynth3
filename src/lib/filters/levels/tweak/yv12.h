// Avisynth v3.0 alpha.  Copyright 2005 Ben Rudiak-Gould et al.
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


#ifndef __AVS_FILTERS_TWEAK_YV12_H__
#define __AVS_FILTERS_TWEAK_YV12_H__

//avisynth include
#include "../tweak.h"


namespace avs { namespace filters { namespace tweak {



/////////////////////////////////////////////////////////////////////////////
//  tweak::YV12
//
//  Tweak subclass dealing with the YV12 color space
//
class YV12 : public Tweak
{

protected:  //constructors

  YV12(PClip child, double hue, double sat, double bright, double cont)
    : Tweak(child , hue, sat, bright, cont) { }

  YV12(PClip child, Tweak const& other)
    : Tweak(child, other) { }


protected:  //Pipeline interface

  virtual CPVideoFrame MakeFrame(PVideoFrame const& source) const;


public:  //factory methods

  static PClip Create(PClip const& child, double hue, double sat, double bright, double cont)
  {
    return PClip( static_cast<Clip *>(new YV12(child, hue, sat, bright, cont)) );
  }

  static PClip Create(PClip const& child, Tweak const& other)
  {
    return PClip( static_cast<Clip *>(new YV12(child, other)) );
  }

};



} } } //namespace avs::filters::tweak

#endif //__AVS_FILTERS_TWEAK_YV12_H__
