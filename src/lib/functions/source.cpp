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
//
// Linking Avisynth statically or dynamically with other modules is making a
// combined work based on Avisynth.  Thus, the terms and conditions of the GNU
// General Public License cover the whole combination.


//avisynth includes
#include "source.h"
#include "../parser/adapt.h"
#include "../linker/core/plugin.h"
#include "../filters/source/colorbars.h"
#ifdef _WIN32
#include "../filters/source/avifilesource.h"
#else
#include "../filters/source/gstreamersource.h"
#endif //_WIN32

namespace avs { namespace functions {


using parser::adapt;
using linker::core::Function;


#ifdef _WIN32
Function Source::aviFileSource( 'c', "AviFileSource", "s", adapt( filters::AviFileSource::Creator() ) );
#else
Function Source::gstreamerSource( 'c', "GstreamerSource", "sii", adapt( filters::GstreamerSource::Creator() ) );
#endif //_WIN32
Function Source::colorBars( 'c', "ColorBars", "ii", adapt( &filters::ColorBars::Create ) );

void Source::RegisterAll(linker::core::Plugin& core)
{
#ifdef _WIN32
  core.Register(&aviFileSource);
#else
  core.Register(&gstreamerSource);
#endif //_WIN32
  core.Register(&colorBars);
}


} } //namespace avs::functions
