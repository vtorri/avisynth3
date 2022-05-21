// Avisynth v3.0 alpha.  Copyright 2003-2007 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_GSTREAMER_MESSAGE_H__
#define __AVS_GSTREAMER_MESSAGE_H__

//avisynth include
#include "forward.h"             //for Message declaration

//boost include
#include <boost/optional.hpp>

//stl include
#include <string>

//gstreamer include
#include <gst/gstelement.h>
#include <gst/gstmessage.h>


namespace avs { namespace gstreamer {


typedef enum
{
  GST_MESSAGE_UNKNOWN           = 0,
  GST_MESSAGE_EOS               = (1 << 0),
  GST_MESSAGE_ERROR             = (1 << 1),
  GST_MESSAGE_WARNING           = (1 << 2),
  GST_MESSAGE_INFO              = (1 << 3),
  GST_MESSAGE_TAG               = (1 << 4),
  GST_MESSAGE_BUFFERING         = (1 << 5),
  GST_MESSAGE_STATE_CHANGED     = (1 << 6),
  GST_MESSAGE_STATE_DIRTY       = (1 << 7),
  GST_MESSAGE_STEP_DONE         = (1 << 8),
  GST_MESSAGE_CLOCK_PROVIDE     = (1 << 9),
  GST_MESSAGE_CLOCK_LOST        = (1 << 10),
  GST_MESSAGE_NEW_CLOCK         = (1 << 11),
  GST_MESSAGE_STRUCTURE_CHANGE  = (1 << 12),
  GST_MESSAGE_STREAM_STATUS     = (1 << 13),
  GST_MESSAGE_APPLICATION       = (1 << 14),
  GST_MESSAGE_ELEMENT           = (1 << 15),
  GST_MESSAGE_SEGMENT_START     = (1 << 16),
  GST_MESSAGE_SEGMENT_DONE      = (1 << 17),
  GST_MESSAGE_DURATION          = (1 << 18),
  GST_MESSAGE_LOST_PREROLL      = (1 << 19),
  GST_MESSAGE_PREROLLED         = (1 << 20),
  GST_MESSAGE_LATENCY           = (1 << 21),
  GST_MESSAGE_ANY               = ~0
} MessageType;


class Message : public GstMessage
{

private:  //declared but not implemented

  Message();
  ~Message();


public:  //get error type

  MessageType GetType ();


public:  //get error message

  boost::optional<std::string> GetError();

};



} } //namespace avs::gstreamer

#endif //__AVS_GSTREAMER_MESSAGE_H__
