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


//avisynth include
#include "copier.h"
#include "sizechanger.h"
#include "../geometry/vecteur.h"


namespace avs { namespace bw {



void SizeChanger::operator ()(BufferWindow& window, Vecteur const& topLeft, Vecteur const& bottomRight) const
{

  Dimension dim(window.GetDimension());
  
  //updating parameters
  dim += bottomRight - topLeft;           //exception if illegal
  int offset = window.offset_ + topLeft % window.pitch();
 
  //conditions where needed to reallocate the Buffer
  if ( ! block::IsAligned(offset)         //alignment requirement no longer met
    || dim.GetWidth() > window.pitch()    //width got bigger than pitch
    || offset < block::Align              //not enough head room
    || offset + dim.GetHeight() * window.pitch() + block::Align > window.buffer_.size() )  //not enough toe room
  {
    BufferWindow temp(dim, window.GetEnvironment());

    Copier()(window, temp, -topLeft);     //copy data at the right place in the new window
    
    window = temp;                        //replace current by new one
  }
  else 
  {             
    window.dim_ = dim;                    //else commit updated parameters
    window.offset_ = offset;
  }

}


} } //namespace avs::bw
