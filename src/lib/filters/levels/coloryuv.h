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


#ifndef __AVS_FILTERS_COLORYUV_H__
#define __AVS_FILTERS_COLORYUV_H__

//avisynth includes
#include "coloryuv/mode.h"
#include "coloryuv/forward.h"
#include "../../clip/caching/concrete.h"
#include "../../clip/onechild/concrete.h"
#include "../../clip/onechild/framemakerpipeline.h"


namespace avs { namespace filters {



/////////////////////////////////////////////////////////////////////////////
//  ColorYUV
//
//
//
class AVS_NOVTABLE ColorYUV : public clip::onechild::FrameMakerPipeline
                            , public clip::onechild::Concrete
                            , public clip::caching::Concrete
{

public:  //structors

  ColorYUV(PClip const& child);

  //generated destructor is fine


public:  //Clip general interface

  virtual CPVideoInfo GetVideoInfo() const { return GetChildVideoInfo(); }


protected:  //implementation helpers

  void Count(VideoFrame const& frame, ByteCounter& y, ByteCounter& u, ByteCounter& v) const;

  void Apply(VideoFrame& frame, coloryuv::LumaMap const& y, coloryuv::ChromaMap const& u, coloryuv::ChromaMap const& v) const;

  //get the adjust methods used by LumaMap and ChromaMap given a mode
  static int (*AdjustY(coloryuv::Mode mode))(int);
  static int (*AdjustU(coloryuv::Mode mode))(int);
  static int (*AdjustV(coloryuv::Mode mode))(int);


public:  //factory method

/*  static PClip Create(PClip const& child, Levels const& Y, float gamma, Levels const& U, Levels const& V
                      InputParameters Y,
                      InputParameters U,
                      InputParameters V,
                      std::string const& _levels,
                      std::string const& _opt,
                      std::string const& _matrix,
                      bool _analyze, bool _autowhite, bool _autogain);*/
};


} } //namespace avs::filters

#endif //__AVS_FILTERS_COLORYUV_H__
