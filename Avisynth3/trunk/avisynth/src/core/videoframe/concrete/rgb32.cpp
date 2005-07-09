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
#include "rgb32.h"
#include "../../colorspace/get.h"
#include "../../../utility/bytemap.h"
#include "../../../utility/bytecounter.h"


namespace avs { namespace videoframe { namespace concrete {



RGB32::RGB32(Dimension const& dim, FrameType type, PEnvironment const& env)
: Interleaved( colorspace::Get::RGB32(), dim, type, env ) { }


RGB32::RGB32(Dimension const& dim, FrameType type, BufferWindow const& main)
  : Interleaved( colorspace::Get::RGB32(), dim, type, main ) { }



CPVideoFrame RGB32::clone() const 
{ 
  return CPVideoFrame( static_cast<VideoFrame *>(new RGB32(*this)) ); 
}


PColorSpace RGB32::GetColorSpace() const { return colorspace::Get::RGB32(); }


void RGB32::ApplyToR(ByteMap const& r) { r.ApplyTo<4>( WriteTo(NOT_PLANAR) ); }
void RGB32::ApplyToG(ByteMap const& g) { g.ApplyTo<4>( WriteTo(NOT_PLANAR) += 1 ); }
void RGB32::ApplyToB(ByteMap const& b) { b.ApplyTo<4>( WriteTo(NOT_PLANAR) += 2 ); }
void RGB32::ApplyToA(ByteMap const& a) { a.ApplyTo<4>( WriteTo(NOT_PLANAR) += 3 ); }



} } } //namespace avs::vframe::concrete
