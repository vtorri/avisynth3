// Avisynth v3.0 alpha.  Copyright 2005 Ben Rudiak-Gould et al.
// http://www.avisynth.org
// This file is Copyleft Vincent Torri

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

#ifndef __AVS_FILTERS_SOURCE_GSTREAMER_FACTORY_H__
#define __AVS_FILTERS_SOURCE_GSTREAMER_FACTORY_H__

//avisynth include
#include "forward.h"
#include "../../../core/forward.h"
#include "../../../core/geometry/dimension.h"

//boost include
#include "../../../core/cow_shared_ptr.h"

//stl include
#include <sstream>
#include <string>
#include <deque>

// gstreamer include
#include <gst/gstelement.h>
#include <gst/gstpad.h>


namespace avs { namespace filters { namespace source { namespace gstreamer {

  struct PipelineDestructor
  {
    void operator () (GstElement *pipeline)
    {
    }
  };

  
class Factory
{
private:
  
  PPipeline      pipeline_;
  PStreamChooser videoChooser_;
  PStreamChooser audioChooser_;

  PVideoInfo vi_;

  // test 
  unsigned int end_test;
  
public:  //structors
  Factory(std::string const& name,
	  int video_stream_index = 0,
	  int audio_stream_index = 0);
  ~Factory() {};

public: // Access methods

  PPipeline GetPipeline() { return pipeline_; }

  PVideoInfo GetVideoInfo() { return vi_; }

/*   CPVideoInfo FillVideoInfo() const; */


public: // Fill the informations;
  
  void Factory::SetPads (Pad& pad);

private: // Fill the informations;
  
  void SetStreamLength();
  

};
  
  

} } } } // namespace avs::filters::source::gstreamer


#endif //__AVS_FILTERS_SOURCE_GSTREAMER_FACTORY_H__
