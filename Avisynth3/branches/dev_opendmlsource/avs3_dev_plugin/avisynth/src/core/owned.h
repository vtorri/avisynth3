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


#ifndef __AVS_OWNED_H__
#define __AVS_OWNED_H__

//avisynth includes
#include "forward.h"       //for PEnvironment
#include "../define.h"     //for AVS_NOVTABLE


namespace avs {



/////////////////////////////////////////////////////////////////////////////////////
//  Owned
//
//  interface was introduced to avoid having GetEnvironment defined twice
//  when a Clip is himself his Cache FrameMaker
//
class AVS_NOVTABLE Owned
{

public:  //structors

  Owned() { }
  virtual ~Owned() { }


public:  //Owned interface

  //fetch owning environment
  virtual PEnvironment const& GetEnvironment() const = 0;

}; 



} //namespace avs

#endif //__AVS_OWNED_H__
