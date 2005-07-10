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


#ifndef __AVS_VFW_EXPORTER_H__
#define __AVS_VFW_EXPORTER_H__

//avisynth includes
#include "../define.h"                 //for AVS_NOVTABLE
#include "../core/forward.h"           //for Dimension typedef


namespace avs { namespace vfw {



///////////////////////////////////////////////////////////////////////////////////
//  vfw::Exporter
//
//
//
class AVS_NOVTABLE Exporter
{

public:  //structors

  Exporter() { }
  virtual ~Exporter() { }


public:  //Exporter interface

  virtual unsigned short GetBitsPerPixel() const = 0;
  virtual unsigned long GetFourCCHandler() const = 0;
  virtual long GetBitmapSize(Dimension const& dim) const = 0;

};



} } //namespace avs::vfw

#endif //__AVS_VFW_EXPORTER_H__
