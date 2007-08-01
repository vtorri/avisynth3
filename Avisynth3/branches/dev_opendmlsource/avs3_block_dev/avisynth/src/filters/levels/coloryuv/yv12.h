// Avisynth v3.0 alpha.  Copyright 2004 Ben Rudiak-Gould et al.
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


#ifndef __AVS_FILTERS_COLORYUV_YV12_H__
#define __AVS_FILTERS_COLORYUV_YV12_H__

//avisynth include
#include "../coloryuv.h"


namespace avs { namespace filters { namespace coloryuv {



/////////////////////////////////////////////////////////////////////////////
//  coloryuv::YV12
//
//  ColorYUV subclass dealing with the YV12 color space
//  FIXME: could be templated into Planar class
class YV12 : public ColorYUV
{

public:  //structors

  YV12(PClip const& child, Levels const& Y, Levels const& U, Levels const& V, float gamma, Mode mode, bool coring, bool autoWhite, bool autoGain)
    : ColorYUV( child, Y, U, V, gamma, mode, coring, autoWhite, autoGain ) { }

  //cloning constructor
  YV12(PClip const& child, ColorYUV const& other)
    : ColorYUV( child, other ) { }

  //analyze constructor
  YV12(PClip const& child)
    : ColorYUV( child ) { }


private:  //ColorYUV interface

  virtual void Count(VideoFrame const& frame, ByteCounter& y, ByteCounter& u, ByteCounter& v) const;

  virtual void ApplyMaps(VideoFrame& frame, coloryuv::LumaMap const& y, coloryuv::ChromaMap const& u, coloryuv::ChromaMap const& v) const;

};


} } } //namespace avs::filters::coloryuv

#endif //__AVS_FILTERS_COLORYUV_YV12_H__
