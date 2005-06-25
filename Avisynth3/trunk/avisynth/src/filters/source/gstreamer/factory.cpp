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


#ifndef _WIN32

//avisynth includes
#include "factory.h"
#include "pipeline.h"
#include "../../../core/videoinfo.h"
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


static void DetectPadsCallback(GObject * obj, GstPad * pad, gboolean last, void * data)
{
  static_cast<Factory *>(data)->PadDetected( static_cast<avs::gstreamer::Pad&>(*pad) );
}
  
static void NoMorePadsCallBack(GObject * obj, GstPad * pad, gboolean last, void * data)
{
  static_cast<avs::gstreamer::Element *>(data)->SetStatePaused();
}


void NotifyVideoCapsCallBack(GObject * o, GParamSpec * pspec, void * data)
{
  avs::gstreamer::PStructure structure = static_cast<avs::gstreamer::Object *>(o)->AsPad().GetNegotiatedStructure();

  static_cast<Factory *>(data)->Set(static_cast<avs::gstreamer::structure::Video const&>(*structure));
}

void NotifyAudioCapsCallBack(GObject * o, GParamSpec * pspec, void * data)
{
  avs::gstreamer::PStructure structure = static_cast<avs::gstreamer::Object *>(o)->AsPad().GetNegotiatedStructure();
  
  static_cast<Factory *>(data)->Set(static_cast<avs::gstreamer::structure::Audio const&>(*structure));
}


//gstreamer initialisation
struct GstInit
{
  GstInit() { gst_init(NULL, NULL); }
} init;


} // anonymous namespace



Factory::Factory(std::string const& name, int videoIndex, int audioIndex)
  : pipeline_( Pipeline::Create(name) )
  , vi_( VideoInfo::Create() )
  , videoChooser_( videoIndex, pipeline_->GetVideoSink(), &NotifyVideoCapsCallBack, *this )
  , audioChooser_( audioIndex, pipeline_->GetAudioSink(), &NotifyAudioCapsCallBack, *this ) 
{ 
  avs::gstreamer::Element& elementPipeline = *pipeline_;

  avs::gstreamer::Object& decoder = pipeline_->GetDecoder();
  avs::gstreamer::SignalHandler padDetected(decoder, "new-decoded-pad", &DetectPadsCallback, this);
  avs::gstreamer::SignalHandler noMorePads(decoder, "no-more-pads", &NoMorePadsCallBack, &elementPipeline);

  elementPipeline.SetStatePlaying();
    
  pipeline_->operator avs::gstreamer::Bin&().Iterate(40);

  pipeline_->SetLengths(*vi_);
}



void Factory::PadDetected(avs::gstreamer::Pad& pad)
{
  char const * mimeType = pad.GetStructure()->GetName();
    
  if ( g_str_has_prefix(mimeType, "video/") )
    videoChooser_.PadDetected(pad);
  else 
    if ( g_str_has_prefix(mimeType, "audio/") )
      audioChooser_.PadDetected(pad);
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

#endif //_WIN32
