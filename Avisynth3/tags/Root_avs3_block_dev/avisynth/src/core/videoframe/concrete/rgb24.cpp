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
#include "rgb24.h"
#include "../../colorspace/get.h"
#include "../../../utility/bytemap.h"
#include "../../../utility/bytecounter.h"


namespace avs { namespace videoframe { namespace concrete {



RGB24::RGB24(Dimension const& dim, FrameType type, PEnvironment const& env)
  : Interleaved( colorspace::Get::RGB24(), dim, type, env ) { }


RGB24::RGB24(Dimension const& dim, FrameType type, BufferWindow const& main)
  : Interleaved( colorspace::Get::RGB24(), dim, type, main ) { }



CPVideoFrame RGB24::clone() const 
{ 
  return CPVideoFrame( static_cast<VideoFrame *>(new RGB24(*this)) ); 
}


PColorSpace RGB24::GetColorSpace() const { return colorspace::Get::RGB24(); }


void RGB24::ApplyToR(ByteMap const& r) { r.ApplyTo<3>( WriteTo('~') ); }
void RGB24::ApplyToG(ByteMap const& g) { g.ApplyTo<3>( WriteTo('~') += 1 ); }
void RGB24::ApplyToB(ByteMap const& b) { b.ApplyTo<3>( WriteTo('~') += 2 ); }



} } } //namespace avs::vframe::concrete
