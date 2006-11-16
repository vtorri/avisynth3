// Avisynth v3.0 alpha.  Copyright 2003-2006 David Pierre - Ben Rudiak-Gould et al.
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


//avisynth include
#include "splitting.h"


namespace avs { namespace memory { namespace holder {



Splitting::Splitting(int32 size, int32 offset, PHolder const& holder, SplitCounter const& counter)
  : size_( size )
  , offset_( offset )
  , holder_( holder )
  , counter_( counter ) { }


Splitting::Splitting(int32 size, int32 offset, Splitting const& other)
  : size_( size )
  , offset_( offset_ )
  , holder_( other.holder_ )
  , counter_( other.counter_ ) { }



bool Splitting::Unique() const
{
  return holder_.use_count() == counter_.Count() && holder_->Unique();
}



PHolder Splitting::Split(int32 splitSize, PHolder& self) const
{
  PHolder result( static_cast<Holder *>(new Splitting(Size() - splitSize, offset_ + splitSize, *this)) );

  //NB: this line can happen to destroy *this, therefore the ordering
  self.reset( static_cast<Holder *>(new Splitting(splitSize, offset_, *this)) );

  return result;
}
  


PHolder Splitting::InitialSplit(int32 splitSize, PHolder& self)
{
  PHolder holder = self;
  SplitCounter counter;

  self.reset( static_cast<Holder *>(new Splitting(splitSize, 0, holder, counter)) );
  
  return PHolder( static_cast<Holder *>(new Splitting(holder->Size() - splitSize, splitSize, holder, counter)) );
}



} } } //namespace avs::memory::holder
