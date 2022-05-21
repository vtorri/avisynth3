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
#include "object.h"
#include "signalhandler.h"

//glib include
#include <glib-object.h>


namespace avs { namespace gstreamer {


unsigned long SignalHandler::Connect(Object * target, char const * signalName, void (*callBack)(), void * data)
{
  if (!target) {
    return 0;
  }
  else {
    return g_signal_connect(G_OBJECT(target), signalName, callBack, data);
  }
}


SignalHandler::~SignalHandler()
{
  if (target_) {
    if ( g_signal_handler_is_connected(G_OBJECT(target_), signalId_) != 0 )
      g_signal_handler_disconnect(G_OBJECT(target_), signalId_);
  }
}



} } //namespace avs::gstreamer

#endif //AVS_HAS_GSTREAMER_SOURCE
