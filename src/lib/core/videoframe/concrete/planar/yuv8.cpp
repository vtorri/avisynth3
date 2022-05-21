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
#include "yuv8.h"
#include "../../../../utility/bytemap.h"
#include "../../../../utility/bytecounter.h"


namespace avs { namespace videoframe { namespace concrete { namespace planar {



void YUV8::CountY(ByteCounter& y) const { y.Count<1>( ReadFrom('Y') ); }
void YUV8::CountU(ByteCounter& u) const { u.Count<1>( ReadFrom('U') ); }
void YUV8::CountV(ByteCounter& v) const { v.Count<1>( ReadFrom('V') ); }


void YUV8::ApplyToY(ByteMap const& y) { y.ApplyTo<1>( WriteTo('Y') ); } 
void YUV8::ApplyToU(ByteMap const& u) { u.ApplyTo<1>( WriteTo('U') ); } 
void YUV8::ApplyToV(ByteMap const& v) { v.ApplyTo<1>( WriteTo('V') ); } 



} } } } //namespace avs::videoframe::concrete::planar
