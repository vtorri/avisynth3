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


#ifndef __AVS_EXPORT_VFW_EXPORTER_INTERLEAVED_H__
#define __AVS_EXPORT_VFW_EXPORTER_INTERLEAVED_H__

//avisynth include
#include "base.h"


namespace avs { namespace export_ { namespace vfw { namespace exporter {



class Interleaved : public Base
{

  unsigned short bytesPerPixel_;


public:  //structors

  Interleaved(PClip const& clip, unsigned long fourCCHandler, unsigned short bytesPerPixel)
    : Base( clip, fourCCHandler )
    , bytesPerPixel_( bytesPerPixel ) { }

  //generated destructor is fine


public:  //vfw::Exporter interface

  virtual unsigned short GetBitsPerPixel() const { return bytesPerPixel_ << 3; }
  virtual long GetBitmapSize(Dimension const& dim) const;


private:  //Base interface

  virtual void ExportFrame(VideoFrame const& frame, BYTE * ptr) const;

};



} } } } //namespace avs::export_::vfw::exporter

#endif //__AVS_EXPORT_VFW_EXPORTER_INTERLEAVED_H__
