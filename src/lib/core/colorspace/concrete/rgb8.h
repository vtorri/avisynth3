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


#ifndef __AVS_COLORSPACE_CONCRETE_RGB8_H__
#define __AVS_COLORSPACE_CONCRETE_RGB8_H__

//avisynth includes
#include "interleaved.h"
#include "../rgb/depth8.h"


namespace avs { namespace colorspace { namespace concrete {



////////////////////////////////////////////////////////////////////////////////////////////
//  RGB8
//
//
//
class AVS_NOVTABLE RGB8 : public Interleaved
                        , public rgb::Depth8
{

public:  //structors

  RGB8(unsigned short bytesPerPixel) : Interleaved( bytesPerPixel ) { }
  //generated destructor is fine


public:  //ColorSpace interface

  virtual bool HasProperty(std::string const& prop) const
  {
    return colorspace::Interleaved::HasProperty(prop) || prop == "RGB";
  }


public:  //fetch exporter(s)

  virtual PExporter GetExporter(PClip const& clip, std::string const& type) const;

};



} } } //namespace avs::colorspace::concrete

#endif //__AVS_COLORSPACE_CONCRETE_RGB8_H__
