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
#include "leftturner.h"


namespace avs { namespace bw {


BufferWindow LeftTurner<1>::operator()(BufferWindow const& source) const
{
  BufferWindow result( source.GetDimension().Turn<1>(), source.GetEnvironment() );

  CWindowPtr src = source.Read();
  WindowPtr dst = result.Write();

  dst.toBottom();

  for( int y = src.height; y-- > 0; src.to(0, 1), dst.to(1, 0) )
    for( int x = src.width; x-- > 0; )
      dst(0, -x) = src(x, 0);

  return result;
}

} } //namespace avs::bw