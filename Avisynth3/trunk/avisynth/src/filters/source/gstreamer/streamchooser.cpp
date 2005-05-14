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


#ifndef _WIN32

//avisynth includes
#include "pad.h"
#include "streamchooser.h"
#include "../../../gstreamer/videostructure.h"
#include "../../../gstreamer/audiostructure.h"


namespace avs { namespace filters { namespace source { namespace gstreamer {



void StreamChooser::PadDetected(Pad& pad, NotifyCapsCallBack callBack, VideoInfo& vi)
{
  if ( index_ == count_ || ! notifyCaps_ )
    notifyCaps.reset( new SignalHandler(G_OBJECT(&pad), "notify-caps", G_CALLBACK(callBack), &vi) );

  ++count_;
}

Pad& StreamChooser::GetChosenPad() const
{
  return *static_cast<Pad*>( GST_PAD(&notifyCaps_->GetObject()) );
}


void StreamChooser::NotifyVideoCapsCallBack(GObject * o, GstPad * pad, int last, void * data)
{
  PVideoStructure vidStruct = 
      boost::static_pointer_cast<gstreamer::VideoStructure>(static_cast<Pad *>(pad)->GetStructure());
  
  vidStruct->FillVideoInfo(*static_cast<VideoInfo*>(data));
}

void StreamChooser::NotifyAudioCapsCallBack(GObject * o, GstPad * pad, int last, void * data)
{
  PVideoStructure audStruct = 
      boost::static_pointer_cast<gstreamer::AudioStructure>(static_cast<Pad *>(pad)->GetStructure());
  
  audStruct->FillVideoInfo(*static_cast<VideoInfo*>(data));
}


/*

PVideoInfo StreamCounter::operator()(Pipeline& pipeline)
{
  GObject& decoder = pipeline.GetDecoder();
  SignalHandler padDetected(decoder, "new-decoded-pad", G_CALLBACK(&DetectPadsCallBack), this);
  SignalHandler noMorePads(decoder, "no-more-pads", G_CALLBACK(&NoMorePadsCallBack), &pipeline_);
  
  pipeline_.SetStatePlaying();       //set pipeline to play mode

  //and iterate to find streams
  //callbacks registered before take care of updating counts
  for ( int i = 40; (i-- > 0) && pipeline_.Iterate(); ) { }

  pipeline_.SetStateReady();

  return std::make_pair(audio_.pad_, video_.pad_);
}


void StreamCounter::PadDetected(Pad& pad)
{
  PGstStructure str = pad.GetStructure();
  
  gchar const * mimeType = gst_structure_get_name(str.get());
  //g_print ("mimetype : %s\n", mimeType);
  
  if ( g_str_has_prefix(mimeType, "video/") )
    video_.PadDetected(pad, G_CALLBACK(&FillVideoInfoCallBack), *vi_);
  else
    if ( g_str_has_prefix(mimeType, "audio/") )
      audio_.PadDetected(pad, G_CALLBACK(&FillAudioInfoCallBack), *vi_););
}

*/

} } } } //namespace avs::filters::source::gstreamer

#endif //_WIN32
