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


#ifndef __AVS_COLORSPACE_CONCRETE_RGB_H__
#define __AVS_COLORSPACE_CONCRETE_RGB_H__

//avisynth includes
#include "interleaved.h"
#include "../../videoframe/rgb.h"
#include "../../exception/colorspace/unsupported.h"


namespace avs { namespace cspace { namespace concrete {



/////////////////////////////////////////////////////////////////////////////////////////
//  RGB24
//
//  RGB24 ColorSpace subclass
//
class RGB24 : public interleaved<3, 1, vframe::RGB24>
{

public:  //ColorSpace interface

  virtual ID id() const { return I_RGB24; }
  virtual char const * GetName() const { return "RGB24"; }
  virtual unsigned long GetFourCC() const { return 0; }    //ie BI_RGB, but I dont to include win headers here for now

  virtual bool HasProperty(Property prop) const
  {
    return prop == P_INTERLEAVED || prop == P_RGB || prop == P_DEPTH8;
  }

};



/////////////////////////////////////////////////////////////////////////////////////////
//  RGB32
//
//  RGB32 ColorSpace subclass
//
class RGB32 : public interleaved<4, 1, vframe::RGB32>
{

public:  //ColorSpace interface

  virtual ID id() const { return I_RGB32; }
  virtual char const * GetName() const { return "RGB32"; }
  virtual unsigned long GetFourCC() const { return 0; }    //ie BI_RGB, but I dont to include win headers here

  virtual bool HasProperty(Property prop) const
  {
    return prop == P_INTERLEAVED || prop == P_RGB || prop == P_DEPTH8;
  }

};



/////////////////////////////////////////////////////////////////////////////////////////
//  RGB45
//
//  RGB32 ColorSpace subclass
//
class RGB45 : public interleaved<8, 2, vframe::RGB45>
{

public:  //ColorSpace interface

  virtual ID id() const { return I_RGB45; }
  virtual char const * GetName() const { return "RGB45"; }
  virtual unsigned long GetFourCC() const { throw exception::cspace::Unsupported(shared_from_this()); }
  virtual long GetBitsPerPixel() const { return 45; }

  virtual bool HasProperty(Property prop) const
  {
    return prop == P_INTERLEAVED || prop == P_RGB || prop == P_DEPTH15;
  }

};




} } } //namespace avs::cspace::concrete

#endif //__AVS_COLORSPACE_CONCRETE_RGB_H__
