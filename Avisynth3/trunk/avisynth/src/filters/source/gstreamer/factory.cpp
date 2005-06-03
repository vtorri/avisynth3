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


// avisynth includes
#include "pad.h"
#include "factory.h"
#include "pipeline.h"
#include "../../../gstreamer/structure.h"
#include "../../../core/videoinfo.h"
#include "../../../core/exception/generic.h"

//gstreamer includes
#include <gst/gst.h>


namespace avs { namespace filters { namespace source { namespace gstreamer {



namespace {


static void DetectPadsCallback(GObject * obj, GstPad * pad, gboolean last, gpointer data)
{
  static_cast<Factory *>(data)->PadDetected( static_cast<Pad&>(*pad) );
}
  
static void NoMorePadsCallBack(GObject * obj, GstPad * pad, gboolean last, gpointer data)
{
  static_cast<Pipeline *>(data)->SetStatePaused();
}


} // anonymous namespace



Factory::Factory(std::string const& name, int videoIndex, int audioIndex)
  : pipeline_( Pipeline::Create(name) )
  , vi_( VideoInfo::Create() )
  , videoChooser_( videoIndex, pipeline_->GetVideoSink(), *this )
  , audioChooser_( audioIndex, pipeline_->GetAudioSink(), *this )  
{
    // Gstreamer init
    gst_init (NULL, NULL);

  // Set the pipeline and the dimension and fps of the video
  // and the rate and channels of the audio
  

  GObject& decoder = pipeline_->GetDecoder();
  SignalHandler padDetected(decoder, "new-decoded-pad", G_CALLBACK(&DetectPadsCallback), this);
  SignalHandler noMorePads(decoder, "no-more-pads", G_CALLBACK(&NoMorePadsCallBack), pipeline_);

  pipeline_->SetStatePlaying ();
    
  for ( int i = 40; i-- > 0 && pipeline_->Iterate(); ) 
  {
    g_print ("I : %d\n", 40-i);
  }


    // Set framecount and samplecount of (resp) the video and
    // the audio streams
    SetStreamLength();

    // Set the pipeline ready to read the video
    Fraction fps = vi_->GetFPS();
    pipeline_->GoToFrame (0, fps);

  }


void Factory::PadDetected(Pad& pad)
{
  std::string mimeType = pad.GetStructure()->GetName();
    
  if ( g_str_has_prefix(mimeType.c_str(), "video/") )
    videoChooser_->PadDetected(pad);
  else 
    if ( g_str_has_prefix(mimeType.c_str(), "audio/") )
      audioChooser_->PadDetected(pad);
}

  
  void Factory::SetStreamLength()
  {
    end_test = 0;
    for ( int i = 30; (i-- > 0) && pipeline_->Iterate(); ) {
      g_print ("l I : %d \n", 40-i);
      Fraction fps = vi_->GetFPS();
      vi_->SetFrameCount (pipeline_->QueryVideoLength (fps));
      vi_->SetSampleCount (pipeline_->QueryAudioLength (vi_->GetSampleRate()));
      
      // Lengths are found, no need to continue searching
      // 	if (AreStreamsFound ())
      // 	  pipeline_->SetStatePaused ();
      
    }
  }


} } } } // namespace avs::filters::source::gstreamer
