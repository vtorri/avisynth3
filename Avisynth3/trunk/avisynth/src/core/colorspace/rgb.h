// Avisynth v3.0 alpha.  Copyright 2003 Ben Rudiak-Gould et al.
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


#ifndef __AVS_CS_RGB_H__
#define __AVS_CS_RGB_H__


//avisynth include
#include "interleaved.h"


namespace avs { namespace cspace {



/////////////////////////////////////////////////////////////////////////////////////////
//  RGB24
//
//  RGB24 ColorSpace subclass
//
class RGB24 : public Interleaved
{

private:  //constructor

  RGB24() { }


public:  //ColorSpace interface

  virtual ID id() const { return I_RGB24; }
  virtual char const * GetName() const { return "RGB24"; }

  virtual bool HasProperty(Property prop) const
  {
    return prop == INTERLEAVED || prop == RGB || prop == DEPTH8;
  }


public:  //Interleaved interface

  virtual int BytesPerPixel() const { return 3; }


public:  //instance

  static RGB24 instance;

};



/////////////////////////////////////////////////////////////////////////////////////////
//  RGB32
//
//  RGB32 ColorSpace subclass
//
class RGB32 : public Interleaved
{

private:  //constructor

  RGB32() { }


public:  //ColorSpace interface

  virtual ID id() const { return I_RGB32; }
  virtual char const * GetName() const { return "RGB32"; }

  virtual bool HasProperty(Property prop) const
  {
    return prop == INTERLEAVED || prop == RGB || prop == DEPTH8;
  }


public:  //Interleaved interface

  virtual int BytesPerPixel() const { return 4; }


public:  //instance

  static RGB32 instance;

};



/////////////////////////////////////////////////////////////////////////////////////////
//  RGB45
//
//  RGB32 ColorSpace subclass
//
class RGB45 : public Interleaved
{

private:  //constructor

  RGB45() { }


public:  //ColorSpace interface

  virtual ID id() const { return I_RGB45; }
  virtual char const * GetName() const { return "RGB45"; }

  virtual bool HasProperty(Property prop) const
  {
    return prop == INTERLEAVED || prop == RGB || prop == DEPTH15;
  }


public:  //Interleaved interface

  virtual int BytesPerPixel() const { return 8; }


public:  //instance

  static RGB45 instance;

};




} } //namespace avs::cspace

#endif //__AVS_CS_RGB_H__