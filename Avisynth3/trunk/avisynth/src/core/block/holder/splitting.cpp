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


//avisynth include
#include "splitting.h"


namespace avs { namespace block { namespace holder {



Splitting::Splitting(boost::shared_ptr<Holder> const& holder, int splitSize, LeftTag tag)
  : offset_( 0 )
  , size_( splitSize )
  , holder_( holder ) { }

Splitting::Splitting(boost::shared_ptr<Holder> const& holder, int splitSize, RightTag tag)
  : offset_( splitSize )
  , size_( holder->Size() - splitSize )
  , holder_( holder ) { }


Splitting::Splitting(Splitting const& other, int splitSize, LeftTag tag)
  : offset_( other.offset_ )
  , size_( splitSize )
  , holder_( other.holder_ ) { }

Splitting::Splitting(Splitting const& other, int splitSize, RightTag tag)
  : offset_( other.offset_ + splitSize )
  , size_( other.size_ - splitSize )
  , holder_( other.holder_ ) { }


boost::shared_ptr<Holder> Splitting::Split(int splitSize, boost::shared_ptr<Holder>& self) const
{
  boost::shared_ptr<Holder> result( static_cast<Holder *>(new Splitting(*this, splitSize, RightTag())) );

  //NB: this line normally happens to destroy this, therefore order matters
  self.reset( static_cast<Holder *>(new Splitting(*this, splitSize, LeftTag())) );

  return result;
}



} } } //namespace av::block::holder
