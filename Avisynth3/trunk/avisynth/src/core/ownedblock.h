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


#ifndef __AVS_OWNEDBLOCK_H__
#define __AVS_OWNEDBLOCK_H__


//avisynth include
#include "block.h"



namespace avs {


//class declaration
class RuntimeEnvironment;

//typedef
typedef boost::shared_ptr<RuntimeEnvironment> PEnvironment;




///////////////////////////////////////////////////////////////////////////////////////
//  OwnedBlock
//
//
//
class OwnedBlock : private Block
{

  PEnvironment env_;


public:  //structors

  OwnedBlock(PEnvironment env, int size);            //basic constructor

  OwnedBlock(PEnvironment env, int size, recycle);   //recycling constructor

  OwnedBlock(OwnedBlock const& other)                //copy constructor
    : Block( other )
    , env_( other.env_ ) { }

  ~OwnedBlock();


public:  //assignment

  OwnedBlock& operator=(OwnedBlock const& other)
  {
    OwnedBlock(other).swap(*this);
    return *this;
  }

  void swap(OwnedBlock& other)
  { 
    Block::swap(other); 
    env_.swap(other.env_);
  }


public:  //reset methods

  void reset(int size) { OwnedBlock( GetEnvironment(), size ).swap(*this); }
  void reset(int size, recycle r) { OwnedBlock( GetEnvironment(), size, r ).swap(*this); }

public:  //read access

  PEnvironment GetEnvironment() const { return env_; }

  BYTE * get() const { return Block::get(); }
  int size() const { return Block::size(); }

  bool unique() const { return Block::unique(); }

};//OwnedBlock





} //namespace avs

#endif //__AVS_OWNEDBLOCK_H__