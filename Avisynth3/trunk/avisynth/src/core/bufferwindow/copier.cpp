// Avisynth v3.0 alpha.  Copyright 2003-2005 Ben Rudiak-Gould et al.
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
#include "../geometry/vecteur.h"
#include "../ownedblock.h"


namespace avs { namespace bw {


void Copier::operator ()(BufferWindow const& src, BufferWindow& dst, Vecteur const& shift) const
{
  Vecteur topLeft = shift >> dst.GetDimension();
  Vecteur bottomRight = ( shift + src.GetDimension() ) >> dst.GetDimension();

  Dimension toCopy = Dimension(bottomRight - topLeft);

  Blitter::Get().Blit( src.Read() += topLeft - shift, dst.Write() += topLeft, toCopy ); 
}


} } //namespace avs::bw
