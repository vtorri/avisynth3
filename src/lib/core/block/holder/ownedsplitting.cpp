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
#include "ownedsplitting.h"


namespace avs { namespace block { namespace holder {



OwnedSplitting::OwnedSplitting(boost::shared_ptr<OwnedHolder> const& holder, int splitSize, split::Left)
  : size_( splitSize )
  , offset_( 0 )
  , holder_( holder ) { }

OwnedSplitting::OwnedSplitting(boost::shared_ptr<OwnedHolder> const& holder, int splitSize, split::Right)
  : size_( holder->Size() - splitSize )
  , offset_( splitSize )
  , holder_( holder ) { }


OwnedSplitting::OwnedSplitting(OwnedSplitting const& other, int splitSize, split::Left)
  : size_( splitSize )
  , offset_( other.offset_ )
  , holder_( other.holder_ ) { }

OwnedSplitting::OwnedSplitting(OwnedSplitting const& other, int splitSize, split::Right)
  : size_( other.Size() - splitSize )
  , offset_( other.offset_ + splitSize )
  , holder_( other.holder_ ) { }


boost::shared_ptr<OwnedHolder> OwnedSplitting::Split(int splitSize, boost::shared_ptr<OwnedHolder>& self) const
{
  boost::shared_ptr<OwnedHolder>
      result( static_cast<OwnedHolder *>(new OwnedSplitting(*this, splitSize, split::Right())) );

  //NB: this line normally happens to destroy this, therefore order matters
  self.reset( static_cast<OwnedHolder *>(new OwnedSplitting(*this, splitSize, split::Left())) );

  return result;
}



} } } //namespace av::block::holder
