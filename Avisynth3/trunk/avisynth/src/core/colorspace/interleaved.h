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


#ifndef __AVS_CS_INTERLEAVED_H__
#define __AVS_CS_INTERLEAVED_H__

//avisynth includes
#include "../colorspace.h"
#include "../exception/nosuchplane.h"


namespace avs { namespace cspace {



////////////////////////////////////////////////////////////////////////////////////////
//  interleaved<bpp, depth8>
//
//
//
template <int bpp_, bool depth8> class interleaved : public ColorSpace
{

public:  //ColorSpace interface

  virtual bool HasPlane(Plane plane) const { return plane == NOT_PLANAR; }

  virtual void ToPlane(int& x, int& y, Plane plane) const
  {
    if ( plane != NOT_PLANAR )
      throw exception::NoSuchPlane(*this, plane);
    x *= bpp_;
  }

public:

  enum { bpp = bpp_, bps = depth8 ? 1 : 2 };

};//Interleaved




} } //namespace avs::cspace

#endif //__AVS_CS_INTERLEAVED_H__
