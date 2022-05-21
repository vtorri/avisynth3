// Avisynth v3.0 alpha.  Copyright 2006 David Pierre - Ben Rudiak-Gould et al.
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


#ifdef _WIN32

//avisynth includes
#include "interleaved.h"
#include "../../core/ownedblock.h"
#include "../../core/bufferwindow.h"


namespace avs { namespace vfw { namespace importer {



PVideoFrame Interleaved::CreateFrame(Dimension const& dim, OwnedBlock const& block) const
{
  //create a 4-bytes aligned frame buffer of the expected size by promoting the block
  //since we respected guards into the block it shouldn't blit
  buffer_window<4, Guard> main( space_->ToPlaneDim(dim, '~'), block, Guard );

  //use space_ to transform the frame buffer into a frame
  //if the size was favorable the conversion 
  //from buffer_window<4, Guard> to BufferWindow may not blit
  return space_->CreateFrame(dim, UNKNOWN, main);
}



} } } //namespace avs::vfw::importer

#endif //_WIN32
