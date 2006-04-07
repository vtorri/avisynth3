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
#include "../../../gstreamer/object.h"
#include "../../../gstreamer/forward.h"        //for Element declaration
#include "../../../gstreamer/pipeline.h"
#include "../../../gstreamer/element.h"
#include "../../../gstreamer/signalhandler.h"

//boost include
#include <boost/shared_ptr.hpp>                //so PPipeline is defined
/* #include <boost/thread.hpp> */

//stl include
#include <string>


namespace avs { namespace filters { namespace source { namespace gstreamer {



////////////////////////////////////////////////////////////////////////////////////////
//  Pipeline
//
//  
//
class Pipeline
{

  avs::gstreamer::PPipeline pipeline_;
  avs::gstreamer::SignalHandler detectPad_;
  avs::gstreamer::Pad * pad_;
  avs::gstreamer::Element * sink_;

public:  //structors

  Pipeline(std::string const& filename);


public:  //access to elements

  avs::gstreamer::Pad * GetPad() { return pad_; }
  avs::gstreamer::Object * GetSink() { return &static_cast<avs::gstreamer::Object&>( *sink_ ); }
  avs::gstreamer::Element& GetPipeline();
  avs::gstreamer::Element& GetDecoder();

  void SetStatePaused();

public:  //factory method

  boost::shared_ptr<avs::gstreamer::Pipeline> BuildPipeline(std::string const& filename);

  void SetSink (int index, char *stream_type);

  void SetFrameCount(VideoInfo& vi);

  void SetSampleCount(VideoInfo& vi);

  static PPipeline Create(std::string const& filename);


public:  //used by callback

  void PadDetected(avs::gstreamer::Pad& pad);
  
};



} } } } // namespace avs::filters::source::gstreamer

#endif // __AVS_FILTERS_SOURCE_GSTREAMER_PIPELINE_H__
