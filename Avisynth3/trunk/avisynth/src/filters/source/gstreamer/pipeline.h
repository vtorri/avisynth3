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


#ifndef __AVS_FILTERS_SOURCE_GSTREAMER_PIPELINE_H__
#define __AVS_FILTERS_SOURCE_GSTREAMER_PIPELINE_H__

//avisynth includes
#include "forward.h"                           //for PPipeline
#include "../../../gstreamer/forward.h"        //for Element declaration
#include "../../../gstreamer/pipeline.h"

//boost include
#include <boost/shared_ptr.hpp>                //so PPipeline is defined

//stl include
#include <string>


namespace avs { namespace filters { namespace source { namespace gstreamer {



////////////////////////////////////////////////////////////////////////////////////////
//  Pipeline
//
//  
//
class Pipeline : public avs::gstreamer::Pipeline
{

public:  //access to elements

  avs::gstreamer::Element& GetDecoder();
  avs::gstreamer::Element& GetVideoSink();
  avs::gstreamer::Element& GetAudioSink();
  

public:  //factory method

  static PPipeline Create(std::string const& name);

  void GoToFrame (int frame_number, Fraction& fps);

  void SetFrameCount(VideoInfo& vi);
  void SetSampleCount(VideoInfo& vi);
  
};



} } } } // namespace avs::filters::source::gstreamer

#endif // __AVS_FILTERS_SOURCE_GSTREAMER_PIPELINE_H__
