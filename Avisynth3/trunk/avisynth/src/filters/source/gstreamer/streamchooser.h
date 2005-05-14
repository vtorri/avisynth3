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
#include "forward.h"                            //for Pad declaration
#include "../../../core/forward.h"              //for VideoInfo
#include "../../../gstreamer/signalhandler.h"

//boost include
#include <boost/scoped_ptr.hpp>

//gstreamer forward declaration
typedef struct _GstPad GstPad;


namespace avs { namespace filters { namespace source { namespace gstreamer {



////////////////////////////////////////////////////////////////////////////////////////
//  StreamChooser
//
//  helper class for Factory
//
class StreamChooser
{

  int count_;
  int index_;

  boost::scoped_ptr<avs::gstreamer::SignalHandler> notifyCaps_;


public:  //structors

  StreamChooser(int index)
    : count_ ( 0 )
    , index_ ( index ) { }

  //generated destructor is fine


public:  //interface  

  typedef void (*NotifyCapsCallBack)(GObject * o, GstPad * pad, int last, void * data);

  void PadDetected(Pad& pad, NotifyCapsCallBack callBack, VideoInfo& vi);

  bool HasChosen() const { return notifyCaps_; }
  Pad& GetChosenPad() const;


public:  //callbacks to use with PadDetected

  static void NotifyVideoCapsCallBack(GObject * o, GstPad * pad, int last, void * data);
  static void NotifyAudioCapsCallBack(GObject * o, GstPad * pad, int last, void * data);

};



} } } } // namespace avs::filters::source::gstreamer

#endif // __AVS_FILTERS_SOURCE_GSTREAMER_STREAMCHOOSER_H__
