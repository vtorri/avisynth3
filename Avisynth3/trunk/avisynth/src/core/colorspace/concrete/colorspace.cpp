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
#include "map.h"
#include "../../colorspace.h"


namespace avs {


namespace {

cspace::concrete::Map map;

} //namespace anonymous



PColorSpace ColorSpace::rgb24() { return map.rgb24(); }
PColorSpace ColorSpace::rgb32() { return map.rgb32(); }
PColorSpace ColorSpace::rgb45() { return map.rgb45(); }
PColorSpace ColorSpace::yuy2() { return map.yuy2(); }
PColorSpace ColorSpace::yv12() { return map.yv12(); }
PColorSpace ColorSpace::yv24() { return map.yv24(); }
PColorSpace ColorSpace::yv45() { return map.yv45(); }


PColorSpace ColorSpace::FromString(std::string const& name) { return map[name]; }



} //namespace avs