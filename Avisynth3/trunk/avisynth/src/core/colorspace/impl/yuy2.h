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


#ifndef __AVS_COLORSPACE_IMPL_YUY2_H__
#define __AVS_COLORSPACE_IMPL_YUY2_H__

//avisynh includes
#include "interleaved.h"
#include "../../videoframe/yuy2.h"
#include "../../exception/colorspace/invalidwidth.h"


namespace avs { namespace cspace { namespace impl {



/////////////////////////////////////////////////////////////////////////////////////////
//  YUY2
//
//  YUY2 ColorSpace subclass
//
class YUY2 : public interleaved<2, 1, vframe::YUY2>
{

public:  //ColorSpace interface

  virtual ID id() const { return I_YUY2; }
  virtual char const * GetName() const { return "YUY2"; }
  virtual unsigned long GetFourCC() const { return MakeFourCC('Y', 'U', 'Y', '2'); }

  virtual bool HasProperty(Property prop) const
  { 
    return prop == P_YUV || prop == P_INTERLEAVED || prop == P_DEPTH8;
  }

  virtual void Check(int x, int y, bool interlaced = false) const 
  {
    InterleavedType::Check(x, y, interlaced);
    if ( x & 1 )                                                        //if x is not even
      throw exception::cspace::InvalidWidth(shared_from_this(), x, 2);  //exception
  }

};


} } } //namespace avs::cspace:::impl

#endif //__AVS_COLORSPACE_IMPL_YUY2_H__
