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


//avisynth includes
#include "../block.h"
#include "../ownedblock.h"
#include "holder/standard.h"
#include "holder/ownedstandard.h"


namespace avs {



block_<block::Align>::block_(int size, bool recycle)
: BaseBlockType( block::holder::Standard::Create(size, recycle) ) { }


owned_block<block::Align>::owned_block(PEnvironment const& env, bool recycle)
  : BaseBlockType( block::holder::OwnedStandard::Create(env, 0, recycle) ) { }

owned_block<block::Align>::owned_block(PEnvironment const& env, int size, bool recycle)
  : BaseBlockType( block::holder::OwnedStandard::Create(env, size, recycle) ) { }



} //namespace avs
