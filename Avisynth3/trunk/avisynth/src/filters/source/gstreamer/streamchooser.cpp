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


//macro include
#include "../config.h"

#ifdef AVS_HAS_GSTREAMER_SOURCE

//avisynth includes
#include "streamchooser.h"
#include "../../../gstreamer/pad.h"
#include "../../../gstreamer/element.h"


namespace avs { namespace filters { namespace source { namespace gstreamer {



StreamChooser::StreamChooser(int index, Element& sink, void (*callBack)(GObject * o, GParamSpec * pspec, void * data), Factory& factory)
  : count_( 0 )
  , index_( index )
  , sourcePad_( NULL )
  , notifyCaps_( *sink.GetPad("sink"), "notify::caps", callBack, &factory ) { }



void StreamChooser::PadDetected(Pad& sourcePad)
{
  //if target stream found or first stream
  if ( index_ == count_ || sourcePad_ == NULL )
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
    //  |                       | sourcePad |---| sinkPad  |      |
    //  |                       +-----------|   |----------+      |
    //  |                                   |   +-----------------+
    //  +-----------------------------------+
    //
    // sourcePad is a ghost pad of the True Pad of the decoder, like
    // a symbolic link to the True Pad.
    // We link sourcePad and sinkPad_
    // We attach the notify::caps signal on sinkPad_
  
    Pad& sinkPad = notifyCaps_.GetTarget().AsPad();

    //unlink current if there is one
    if ( sourcePad_ != NULL )
      sourcePad_->Unlink(sinkPad);

    //link new one
    sourcePad_ = &sourcePad;
    sourcePad_->Link(sinkPad);
  }

  ++count_;   //update stream count
}



} } } } //namespace avs::filters::source::gstreamer

#endif //AVS_HAS_GSTREAMER_SOURCE
