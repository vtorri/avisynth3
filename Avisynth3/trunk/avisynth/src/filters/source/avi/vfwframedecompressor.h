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


#ifdef _WIN32

#ifndef __AVS_FILTERS_AVISOURCE_VFWFRAMEDECOMPRESSOR_H__
#define __AVS_FILTERS_AVISOURCE_VFWFRAMEDECOMPRESSOR_H__

//avisynth includes
#include "framedecompressor.h"
#include "../../../vfw/hic.h"
#include "../../../vfw/bitmapinfoheader.h"

//windows includes
#include <windows.h>
#include <vfw.h>


namespace avs { namespace filters { namespace avisource {



///////////////////////////////////////////////////////////////////////////////////////////////
//  VFWFrameDecompressor
//
//  a decompressor using VFW
//
class VFWFrameDecompressor : public FrameDecompressor
{

  vfw::Hic hic_;                   //smart handle to a VFW decompressor
  vfw::PBitmapInfoHeader input_;
  vfw::BitmapInfoHeader output_;

  //preferred ColorSpaces (names) for output in priority order
  static char const * const preferredColorSpace_[4];


public:  //structors

  //NB: bih is returned containing the output format
  VFWFrameDecompressor(AviSource const& src, vfw::PBitmapInfoHeader const& bih, PColorSpace& space);

  virtual ~VFWFrameDecompressor();


private:  //FrameDecompressor private interface

  virtual OwnedBlock operator()(bool keyframe, bool preroll, OwnedBlock const& block, long bytesRead);

};



} } } //namespace avs::filters::avisource

#endif //__AVS_FILTERS_AVISOURCE_VFWFRAMEDECOMPRESSOR_H__

#endif //_WIN32
