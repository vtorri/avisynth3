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


#ifndef __AVS_FILTERS_RESIZE_H__
#define __AVS_FILTERS_RESIZE_H__

//avisynth includes
#include "forward.h"
#include "subrange.h"
#include "../../clip/caching/concrete.h"
#include "../../clip/onechild/concrete.h"
#include "../../clip/onechild/cachingpipeline.h"


namespace avs { namespace filters {



////////////////////////////////////////////////////////////////////////////////////////
//  Resize
//
//  common base class of resize::Horizontal and resize::Vertical
//
class Resize : public clip::onechild::CachingPipeline
             , public clip::onechild::Concrete
             , public clip::caching::Concrete
{

  CPVideoInfo vi_;
  resize::PFilter filter_;      //filter used
  resize::SubRange subrange_;   //range of the source used


public:  //structors

  Resize(PClip const& child, resize::PFilter const& filter, Dimension const& dim, resize::SubRange const& subrange);

  //generated destructor is fine


public:  //clip general interface

  virtual CPVideoInfo GetVideoInfo() const { return vi_; }


protected:  //Pipeline interface

  virtual CPVideoFrame MakeFrame(CPVideoFrame const& source) const;


private:  //Resize method

  virtual void ResizeFrame(VideoFrame const& source, VideoFrame& target) const = 0;


public:  //read access

  resize::PFilter const& GetFilter() const { return filter_; }
  resize::SubRange const& GetSubRange() const { return subrange_; }

};//Resize




} } //namespace avs::filters

#endif //__AVS_FILTERS_RESIZE_H__
