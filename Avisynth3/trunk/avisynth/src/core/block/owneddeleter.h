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


#ifndef __AVS_BLOCK_OWNEDDELETER_H__
#define __AVS_BLOCK_OWNEDDELETER_H__

//avisynth includes
#include "deleter.h"
#include "../forward.h"


namespace avs { namespace block {



////////////////////////////////////////////////////////////////////////////////////////
//  OwnedDeleter
//
//  custom deleter used by OwnedBlock
//
class OwnedDeleter : public Deleter
{

  PEnvironment env_;


public:  //structors

  OwnedDeleter(PEnvironment const& env, int size, bool recycle); 
  OwnedDeleter(OwnedDeleter const& other);
  ~OwnedDeleter();


private:  //assignement

  OwnedDeleter& operator=(OwnedDeleter const& other);  //private so unusable


public:  //queries

  PEnvironment const& GetEnvironment() const { return env_; }

};


} } //namespace avs::block

#endif //__AVS_BLOCK_OWNEDDELETER_H__
