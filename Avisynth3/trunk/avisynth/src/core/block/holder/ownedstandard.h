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


#ifndef __AVS_BLOCK_HOLDER_OWNEDSTANDARD_H__
#define __AVS_BLOCK_HOLDER_OWNEDSTANDARD_H__

//avisynth include
#include "ownedbase.h"
#include "../align.h"
#include "../recycler.h"


namespace avs { namespace block { namespace holder {



////////////////////////////////////////////////////////////////////////////////////////////////////
//  holder::OwnedStandard
//
//  OwnedHolder implementation used by avisynth
//
class AVS_NOVTABLE OwnedStandard : public OwnedBase
{

  BYTE * ptr_;


public:  //structors

  OwnedStandard(PEnvironment const& env, int size)
    : OwnedBase( env, size )
    , ptr_( Recycler::instance.Acquire(size) ) { }

  //generated destructor is fine, subclass destructor handles memory release


public:  //Holder interface

  virtual BYTE * get() const { return ptr_; }
  virtual OwnedHolder * spawn(int size, bool recycle) const { return Create(GetEnvironment(), size, recycle); }

  enum { Align = block::Align };


public:  //factory method

  static OwnedStandard * Create(PEnvironment const& env, int size, bool recycle);


private:  //concrete inner subclass

  template <bool recycle> struct concrete;

};


template <bool recycle>
struct OwnedStandard::concrete : public OwnedStandard
{
  concrete(PEnvironment const& env, int size) : OwnedStandard(env, size) { }
  virtual ~concrete() { Recycler::instance.Return(get(), size(), recycle); }
};


OwnedStandard * OwnedStandard::Create(PEnvironment const& env, int size, bool recycle)
{
  return recycle ? static_cast<OwnedStandard *>(new concrete<true>(env, size))
                 : static_cast<OwnedStandard *>(new concrete<false>(env, size));
}


} } } //namespace avs::block::holder

#endif //__AVS_BLOCK_HOLDER_OWNEDSTANDARD_H__
