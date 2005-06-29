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


#ifndef __AVS_FILTERS_SOURCE_GSTREAMER_STREAMCHOOSER_H__
#define __AVS_FILTERS_SOURCE_GSTREAMER_STREAMCHOOSER_H__

//avisynth includes
#include "forward.h"                            //for Factory declaration
#include "../../../gstreamer/forward.h"         //for Pad, Element declarations
#include "../../../gstreamer/signalhandler.h"

//some gstreamer forward declarations
typedef struct _GObject GObject;
typedef struct _GParamSpec GParamSpec;


namespace avs { namespace filters { namespace source { namespace gstreamer {



////////////////////////////////////////////////////////////////////////////////////////
//  StreamChooser
//
//  helper class for Factory
//
class StreamChooser
{

  //just to avoid long names below
  typedef avs::gstreamer::Pad Pad;;
  typedef avs::gstreamer::Element Element;

  int count_;
  int index_;
  Pad * sourcePad_;
  avs::gstreamer::SignalHandler notifyCaps_;


public:  //structors

  StreamChooser(int index, Element& sink, void (*callBack)(GObject * o, GParamSpec * pspec, void * data), Factory& factory);

  //generated destructor is fine


public:  //interface  

  void PadDetected(Pad& sourcePad);

  bool HasChosen() const { return sourcePad_ != NULL; }
  Pad& GetChosenPad() const { return *sourcePad_; }

};



} } } } // namespace avs::filters::source::gstreamer

#endif // __AVS_FILTERS_SOURCE_GSTREAMER_STREAMCHOOSER_H__
