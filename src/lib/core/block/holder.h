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


#ifndef __AVS_BLOCK_HOLDER_H__
#define __AVS_BLOCK_HOLDER_H__

//avisynth includes
#include "../forward.h"          //for BYTE
#include "../../define.h"        //for AVS_NOVTABLE macro


namespace avs { namespace block {



//////////////////////////////////////////////////////////////////////////////////////
//  Holder
//
//  polymorphic interface of the block template memory holder
//
//  NB: the block::base constructor expects an Align enum member to be defined
//      which must be provided by subclassses
//
class AVS_NOVTABLE Holder
{

public:  //structors

  Holder() { }
  virtual ~Holder() { }


public:  //Holder interface

  virtual int Size() const = 0;
  virtual BYTE * Get() const = 0;

  virtual bool Unique() const = 0;

  virtual boost::shared_ptr<Holder> Split(int splitSize, boost::shared_ptr<Holder>& self) const;

};


} } //namespace avs::block

#endif //__AVS_BLOCK_HOLDER_H__
