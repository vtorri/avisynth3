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

//gstreamer includes
#include <gst/gstelement.h>


namespace avs { namespace filters { namespace source { namespace gstreamer {



StreamChooser::StreamChooser(int index, GstElement& sink, void (*callBack)(GObject * o, GParamSpec *pspec, void * data) )
  : count_( 0 )
  , index_( index )
  , currentPad_( NULL )
  , notifyCaps_( *GST_OBJECT(gst_element_get_pad(&sinkPad, "sink")), callBack, &factory ) { }



void StreamChooser::PadDetected(Pad& sourcePad, NotifyCapsCallBack callBack, VideoInfo& vi)
{
  //if target stream found or first stream
  if ( index_ == count_ || currentPad_ == NULL )
  {
    //  +-----------------------------------+
    //  |             Decodebin             |
    //  |                                   |
    //  |   +---------------+               |
    //  |   |               |               |
    //  |   |    Decoder    |               |
    //  |   |               |               |
    //  |   |    +----------|               |
    //  |   |    | True Pad |\              |
    //  |   |    +----------| \             |   +-----------------+
    //  |   +---------------+  \            |   |            Sink |
    //  |                       +-----------|   |----------+      |
    //  |                       | sourcePad |---| sinkPad_ |      |
    //  |                       +-----------|   |----------+      |
    //  |                                   |   +-----------------+
    //  +-----------------------------------+
    //
    // sourcePad is a ghost pad of the True Pad of the decoder, like
    // a symbolic link to the True Pad.
    // We link sourcePad and sinkPad_
    // We attach the notify::caps signal on sinkPad_
  
    //unlink current if there is one
    if ( currentPad_ != NULL )
	    gst_pad_unlink( GST_PAD(currentPad_), GST_PAD(&notifyCaps_.GetTarget()) );

    //link new one
    currentPad_ = &sourcePad;
    gst_pad_link( GST_PAD(currentPad_), GST_PAD(&notifyCaps_.GetTarget()) );    
  }

  ++count_;   //update stream count
}




void StreamChooser::NotifyVideoCapsCallBack(GObject * o, GParamSpec *pspec, void * data)
{
  boost::shared_ptr<VideoStructure const> vidStruct = 
      boost::static_pointer_cast<VideoStructure const>(static_cast<Pad *>(GST_PAD(o))->GetNegotiatedStructure());
  
  vidStruct->FillVideoInfo(*static_cast<VideoInfo*>(data));
}

void StreamChooser::NotifyAudioCapsCallBack(GObject * o, GParamSpec *pspec, void * data)
{
  boost::shared_ptr<AudioStructure const> audStruct = 
      boost::static_pointer_cast<AudioStructure const>(static_cast<Pad *>(GST_PAD(o))->GetNegotiatedStructure());
  
  audStruct->FillVideoInfo(*static_cast<VideoInfo*>(data));
}


} } } } //namespace avs::filters::source::gstreamer

#endif //_WIN32
