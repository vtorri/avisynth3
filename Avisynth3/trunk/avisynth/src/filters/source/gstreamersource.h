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


#ifndef __AVS_FILTERS_GSTREAMERSOURCE_H__
#define __AVS_FILTERS_GSTREAMERSOURCE_H__

//avisynth includes
#include "gstreamer/forward.h"                //for PPipeline
#include "../../core/forward.h"               //for PImporter
#include "../../clip/nochild.h"
#include "../../gstreamer/buffer.h"           //for PBuffer
#include "../../clip/framemaker/concrete.h"
#include "../../gstreamer/signalhandler.h"

//stl include
#include <string>


typedef struct _GstBuffer GstBuffer;
typedef struct _GObject GObject;
typedef struct _GstPad GstPad;


namespace avs { namespace filters {



///////////////////////////////////////////////////////////////////////////////////////
//  GstreamerSource
//
//
//
class GstreamerSource : public clip::NoChild
                      , public clip::framemaker::Concrete
{

  CPVideoInfo vi_;
  PImporter importer_;
  source::gstreamer::PPipeline pipelineVideo_;
  source::gstreamer::PPipeline pipelineAudio_;

public:  //structors

  GstreamerSource(source::gstreamer::Factory const& factory, PEnvironment const& env);

  //generated destructor is fine


public:  //Clip general interface

  virtual CPVideoInfo GetVideoInfo() const { return vi_; }

  virtual BYTE * GetAudio(BYTE * buffer, long long start, long count) const;


public:  //FrameMaker interface

  virtual CPVideoFrame operator()(long int n) const;


public:  //factory functor

  static PClip Create(std::string const& fileName, int videoIndex, int audioIndex, PEnvironment const& env);

  struct Creator
  {
    PClip operator()(std::string const& fileName, int videoIndex, int audioIndex, PEnvironment const& env) const { return Create(fileName, videoIndex, audioIndex, env); }
  };

};



} } //namespace avs::filters

#endif //__AVS_FILTERS_GSTREAMERSOURCE_H__
