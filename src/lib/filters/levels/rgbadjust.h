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


#ifndef __AVS_FILTERS_RGBADJUST_H__
#define __AVS_FILTERS_RGBADJUST_H__

//avisynth includes
#include "rgbadjust/scale.h"
#include "../../clip/onechild/concrete.h"
#include "../../clip/framemaker/concrete.h"
#include "../../clip/onechild/framemakerpipeline.h"


namespace avs { namespace filters {




///////////////////////////////////////////////////////////////////////////////////////////
//  RGBAdjust
//
//
//
class RGBAdjust : public clip::onechild::FrameMakerPipeline
                , public clip::onechild::Concrete
                , public clip::framemaker::Concrete
{

  rgbadjust::Scale r_, g_, b_, a_;


public:  //structors

  RGBAdjust(PClip const& child, double r, double g, double b, double a);

  RGBAdjust(PClip const& child, RGBAdjust const& other);

  //generated destructor is fine


public:  //Clip general interface

  virtual CPVideoInfo GetVideoInfo() const { return GetChildVideoInfo(); }

  virtual BYTE * GetAudio(BYTE * buffer, long long start, long count) const { return GetChildAudio(buffer, start, count); }


public:  //child changing clone

  virtual PClip clone(PClip const& child) const;


protected:  //Pipeline interface

  virtual CPVideoFrame MakeFrame(PVideoFrame const& source) const;


public:  //factory function

  static PClip Create(PClip const& child, double r, double g, double b, double a)
  {
    return PClip( static_cast<Clip *>(new RGBAdjust(child, r, g, b, a)) );
  }

};



} } //namespace avs::filters

#endif //__AVS_FILTERS_RGBADJUST_H__
