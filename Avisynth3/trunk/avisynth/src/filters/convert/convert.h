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


#ifndef __AVS_FILTERS_CONVERT_H__
#define __AVS_FILTERS_CONVERT_H__

//avisynth includes
#include "../../clip/onechild/concrete.h"
#include "../../clip/framemaker/concrete.h"
#include "../../clip/onechild/framemakerpipeline.h"


namespace avs { namespace filters {



////////////////////////////////////////////////////////////////////////////
//  Convert
//
//  base class for ColorSpace conversions filters
//
class Convert : public clip::onechild::FrameMakerPipeline
              , public clip::onechild::Concrete
              , public clip::framemaker::Concrete
{

  CPVideoInfo vi_;


public:  //structors

  Convert(PClip const& child, PColorSpace const& target);

  //generated destructor is fine


public:  //clip general interface

  virtual CPVideoInfo GetVideoInfo() const { return vi_; }


protected:  //Pipeline interface

  virtual CPVideoFrame MakeFrame(PVideoFrame const& source) const;


private:  //Convert method

  virtual void ConvertFrame(VideoFrame const& source, VideoFrame& target) const = 0;


protected:  //implementation helper

  static PClip FromExternal(PClip const& clip, PColorSpace const& target);


public:  //factory method

  static PClip Create(PClip const& clip, PColorSpace const& target);

};



} } //namespace avs::filters

#endif //__AVS_FILTERS_CONVERT_H__
