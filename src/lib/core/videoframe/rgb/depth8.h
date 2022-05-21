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


#ifndef __AVS_VIDEOFRAME_RGB_DEPTH8_H__
#define __AVS_VIDEOFRAME_RGB_DEPTH8_H__

//avisynth include
#include "../../videoframe.h"


namespace avs {

//declarations
class ByteMap;
class ByteCounter;

namespace videoframe { namespace rgb {



//////////////////////////////////////////////////////////////////////////////////////////////
//  vframe::rgb::Depth8
//
//
//
class AVS_NOVTABLE Depth8 : public virtual VideoFrame
{

public:  //rgb::Depth8 interface

  //virtual void Count(ByteCounter& r, ByteCounter& g, ByteCounter& b, ByteCounter& a) const = 0;

  virtual void ApplyToR(ByteMap const& r) = 0;
  virtual void ApplyToG(ByteMap const& g) = 0;
  virtual void ApplyToB(ByteMap const& b) = 0;
  virtual void ApplyToA(ByteMap const& a) = 0;

};



} } } //namespace avs::videoframe::rgb

#endif //__AVS_VIDEOFRAME_RGB_DEPTH8_H__
