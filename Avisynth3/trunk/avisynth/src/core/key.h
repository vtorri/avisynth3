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


#ifndef __AVS_KEY_H__
#define __AVS_KEY_H__

//avisynth include
#include "forward.h"             //for PKey typedef

//boost include
#include <boost/shared_ptr.hpp>  //so its defined 


namespace avs {



////////////////////////////////////////////////////////////////////////////////
//  Key
//
//  used as name/type for (frame) properties
//
class Key
{

public:  //structors

  Key() { }
  virtual ~Key() { }


public:  //Key interface

  virtual bool operator==(Key const& other) const { return &other == this; }
  virtual unsigned hash() const { return reinterpret_cast<unsigned>(this); }


public:  //useful functors

  struct Comparator
  {
    bool operator()(PKey const& left, PKey const& right) const { return *left == *right; }
  };

  struct Hasher
  {
    unsigned operator()(PKey const key) const { return key->hash(); }
  };

};


} //namespace avs

#endif //__AVS_KEY_H__
