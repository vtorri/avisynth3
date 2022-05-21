// Avisynth v3.0 alpha.  Copyright 2003-2005 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_COLORSPACE_IMPORTER_H__
#define __AVS_COLORSPACE_IMPORTER_H__

//avisynth includes
#include "../forward.h"             //for PColorSpace, CPVideoFrame, Dimension, owned_block template
#include "../frametype.h"
#include "../../define.h"           //for AVS_NOVTABLE

//boost include
#include <boost/shared_ptr.hpp>     //so PColorSpace, CPVideoFrame are defined


namespace avs { namespace colorspace { 



////////////////////////////////////////////////////////////////////////////////////////
//  colorspace::Importer
//
//  tranforms raw (uncompressed) data into an avs VideoFrame
//
class AVS_NOVTABLE Importer
{

public:  //structors

  Importer() { }
  virtual ~Importer() { }


public:  //Importer interface

  //fetchs target colorspace
  virtual PColorSpace GetColorSpace() const = 0;

  //creates a frame of given dimension using data from block
  virtual CPVideoFrame CreateFrame(Dimension const& dim, FrameType type, owned_block<1> const& block, long offset) const = 0;

};



} } //namespace avs::colorspace

#endif //__AVS_COLORSPACE_IMPORTER_H__
