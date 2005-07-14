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


#ifndef __AVS_COLORSPACE_CONCRETE_INTERLEAVED_H__
#define __AVS_COLORSPACE_CONCRETE_INTERLEAVED_H__

//avisynth include
#include "../interleaved.h"


namespace avs { namespace colorspace { namespace concrete {



////////////////////////////////////////////////////////////////////////////////////////
//  concrete::Interleaved
//
//
//
class AVS_NOVTABLE Interleaved : public colorspace::Interleaved
{

  long bpp_;           //bytes per pixel


public:  //structors

  Interleaved(long bpp) : bpp_( bpp ) { }
  //generated destructor is fine


public:  //ColorSpace interface

  virtual void Check(long /*x*/, long y, bool interlaced) const;
  virtual void ToPlane(long& x, long& /*y*/, char plane) const;


public:  //fetch exporter(s)

  virtual PExporter GetExporter(std::string const& type) const;

};



} } } //namespace avs::colorspace::concrete

#endif //__AVS_COLORSPACE_CONCRETE_INTERLEAVED_H__
