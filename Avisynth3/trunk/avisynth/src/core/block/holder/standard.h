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


#ifndef __AVS_BLOCK_HOLDER_STANDARD_H__
#define __AVS_BLOCK_HOLDER_STANDARD_H__

//avisynth include
#include "base.h"
#include "../align.h"
#include "../recycler.h"


namespace avs { namespace block { namespace holder {



///////////////////////////////////////////////////////////////////////////////////////////////
//  holder::Standard
//
//  Holder implementation used by avisynth
//
class AVS_NOVTABLE Standard : public Base
{

  BYTE * ptr_;


public:  //structors

  Standard(int size)
    : Base( size )
    , ptr_( Recycler::instance.Acquire(size) ) { }

  //generated destructor is fine, subclass destructor takes care of releasing the memory


public:  //Holder interface

  virtual BYTE * get() const { return ptr_; }
  virtual Holder * spawn(int size, bool recycle) const { return Create(size, recycle); }

  enum { Align = block::Align };


public:  //factory method

  static Standard * Create(int size, bool recycle);  


private:  //concrete inner subclass

  template <bool recycle> struct concrete;

};


template <bool recycle>
struct Standard::concrete : public Standard
{
  concrete(int size) : Standard(size) { }
  virtual ~concrete() { Recycler::instance.Return(get(), size(), recycle); }
};


Standard * Standard::Create(int size, bool recycle)
{
  return recycle ? static_cast<Standard *>(new concrete<true>(size)) 
                 : static_cast<Standard *>(new concrete<false>(size));
}


} } } //namespace avs::block::holder

#endif //__AVS_BLOCK_HOLDER_STANDARD_H__
