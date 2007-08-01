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


#ifndef __AVS_EXPORT_VFW_EXPORTER_H__
#define __AVS_EXPORT_VFW_EXPORTER_H__

//avisynth includes
#include "../../core/forward.h"              //for Dimension typedef
#include "../../core/colorspace/exporter.h"


namespace avs { namespace export_ { namespace vfw {



///////////////////////////////////////////////////////////////////////////////////
//  vfw::Exporter
//
//
//
class AVS_NOVTABLE Exporter : public colorspace::Exporter
{

public:  //vfw::Exporter interface

  virtual unsigned short GetBitsPerPixel() const = 0;
  virtual unsigned long GetFourCCHandler() const = 0;
  virtual long GetBitmapSize(Dimension const& dim) const = 0;


  virtual long GetPreviousKeyFrame(long n) const = 0;
  virtual long GetNextKeyFrame(long n) const = 0;

  virtual long GetFrameSize(long n) const = 0;
  virtual void ExportFrame(long n, BYTE * ptr) const = 0;

};



} } } //namespace avs::export_::vfw

#endif //__AVS_EXPORT_VFW_EXPORTER_H__
