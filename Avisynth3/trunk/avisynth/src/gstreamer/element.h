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


#ifndef __AVS_GSTREAMER_ELEMENT_H__
#define __AVS_GSTREAMER_ELEMENT_H__

//avisynth include
#include "forward.h"             //for Pad, Object, Pipeline declarations

//gstreamer include
#include <gst/gstelement.h>


namespace avs { namespace gstreamer {


 
class Element : public GstElement
{

private:  //declared but not implemented

  Element();
  ~Element();


public:  //access to pads

  Pad * GetPad(char const * name);


public:  //link

  bool Link(Element& target) { return gst_element_link(this, &target) != 0; }


public:  //state stuff

  void SetStateNull() { gst_element_set_state( this, GST_STATE_NULL ); }
  void SetStateReady() { gst_element_set_state( this, GST_STATE_READY ); }
  void SetStatePaused() { gst_element_set_state( this, GST_STATE_PAUSED ); }
  void SetStatePlaying() { gst_element_set_state( this, GST_STATE_PLAYING ); }


public:

  void Seek(GstSeekType type, long long time) { gst_element_seek(this, type, time); }

  bool QueryTotal(GstFormat fmt, long long& result)
  {
    return gst_element_query(this, GST_QUERY_TOTAL, &fmt, &result) != 0;
  }


public:  //casts

  operator Object&();

  Pipeline& AsPipeline();


public:  //factory method

  static Element * Create(char const * type, char const * name);

};



} } //namespace avs::gstreamer

#endif //__AVS_GSTREAMER_ELEMENT_H__
