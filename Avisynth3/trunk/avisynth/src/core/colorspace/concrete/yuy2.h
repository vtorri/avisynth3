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


#ifndef __AVS_COLORSPACE_CONCRETE_YUY2_H__
#define __AVS_COLORSPACE_CONCRETE_YUY2_H__

//avisynth includes
#include "interleaved.h"
#include "../yuv/depth8.h"
#include "../../videoframe/yuy2.h"


namespace avs { namespace cspace { namespace concrete {



/////////////////////////////////////////////////////////////////////////////////////////
//  YUY2
//
//  YUY2 ColorSpace subclass
//
class YUY2 : public interleaved<2, 1, vframe::YUY2>
           , public yuv::Depth8
{

public:  //ColorSpace interface

  virtual ID id() const { return I_YUY2; }
  virtual char const * GetName() const { return "YUY2"; }
  virtual unsigned long GetFourCC() const { return MakeFourCC('Y', 'U', 'Y', '2'); }

  virtual bool HasProperty(Property prop) const;

  virtual void Check(int x, int y, bool interlaced = false) const;


public:  //yuv::Depth8 interface

  virtual void Count(VideoFrame const& frame, ByteCounter& y, ByteCounter& u, ByteCounter& v) const;

  virtual void Apply(VideoFrame& frame, ByteMap const& y, ByteMap const& u, ByteMap const& v) const;

};


} } } //namespace avs::cspace:::concrete

#endif //__AVS_COLORSPACE_CONCRETE_YUY2_H__
