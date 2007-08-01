// Avisynth v3.0 alpha.  Copyright 2005 David Pierre - Ben Rudiak-Gould et al.
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


//macro include
#include "../../../config.h"

#ifdef AVS_HAS_GSTREAMER_SOURCE

//avisynth includes
#include "factory.h"
#include "pipeline.h"
#include "../../../core/videoinfo.h"
#include "../../../core/colorspace.h"
#include "../../../gstreamer/bin.h"
#include "../../../gstreamer/pad.h"
#include "../../../gstreamer/object.h"
#include "../../../gstreamer/element.h"
#include "../../../gstreamer/structure/video.h"
#include "../../../gstreamer/structure/audio.h"

//gstreamer include
#include <gst/gst.h>              //for gst_init


namespace avs { namespace filters { namespace source { namespace gstreamer {



namespace {


//gstreamer initialisation
struct GstInit
{
  GstInit() { gst_init(NULL, NULL); }
} init;


} // anonymous namespace



Factory::Factory(std::string const& name, int videoIndex, int audioIndex)
  : pipelineVideo_( Pipeline::Create(name) )
  , pipelineAudio_( Pipeline::Create(name) )
  , vi_( VideoInfo::Create() )
{
  /* video */
  pipelineVideo_->SetStatePaused();
  pipelineVideo_->SetSink( videoIndex, "video/");
  if ( pipelineVideo_->GetPad() ) {
    avs::gstreamer::PStructure structure_video = pipelineVideo_->GetPad()->GetStructure();
    Set( static_cast<avs::gstreamer::structure::Video const&>(*structure_video) );
    pipelineVideo_->SetFrameCount(*vi_);
  }

  /* audio */
  pipelineAudio_->SetStatePaused();
  pipelineAudio_->SetSink( audioIndex, "audio/");
  if ( pipelineAudio_->GetPad() ) {
    avs::gstreamer::PStructure structure_audio = pipelineAudio_->GetPad()->GetStructure();
    Set( static_cast<avs::gstreamer::structure::Audio const&>(*structure_audio) );
    pipelineAudio_->SetSampleCount(*vi_);
  }

  /* display */
  if (vi_->HasVideo()) {
    g_print ("size         : %dx%d\n", vi_->GetWidth(), vi_->GetHeight());
    g_print ("frame count  : %d\n", vi_->GetFrameCount());
    g_print ("fps          : %d/%d\n", vi_->GetFPSNumerator(), vi_->GetFPSDenominator());
    PColorSpace const& cp = vi_->GetColorSpace();
    g_print ("colorspace   : %s\n", cp->GetName());
  }
  else {
    g_print ("No video...\n");
  }
  if (vi_->HasAudio()) {
    g_print ("sample count : %lld\n", vi_->GetSampleCount());
    g_print ("sample rate  : %d\n", vi_->GetSampleRate());
    g_print ("channels     : %d\n", vi_->GetChannelCount());
  }
  else {
    g_print ("No audio...\n");
  }
}

void Factory::Set(avs::gstreamer::structure::Video const& video)
{
  importer_ = video.SetVideoInfo(*vi_);
}

void Factory::Set(avs::gstreamer::structure::Audio const& audio)
{
  audio.SetVideoInfo(*vi_);
}



} } } } // namespace avs::filters::source::gstreamer

#endif //AVS_HAS_GSTREAMER_SOURCE
