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


//avisynth includes
#include "holder.h"
#include "holder/splitting.h"


namespace avs { namespace block {



boost::shared_ptr<Holder> Holder::Split(int splitSize, boost::shared_ptr<Holder>& self) const
{
  using holder::Splitting;
  using holder::Splitting::LeftTag;
  using holder::Splitting::RightTag;

  boost::shared_ptr<Holder> result( static_cast<Holder *>(new Splitting(self, splitSize, RightTag())) );

  //NB: this line normally happens to destroy this, therefore order matters
  self.reset( static_cast<Holder *>(new Splitting(self, splitSize, LeftTag())) );

  return result;
}



} } //namespace avs::block
