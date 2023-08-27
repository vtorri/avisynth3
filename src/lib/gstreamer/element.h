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


#ifndef __AVS_GSTREAMER_ELEMENT_H__
#define __AVS_GSTREAMER_ELEMENT_H__

//avisynth include
#include "forward.h"             //for Pad, Object, Pipeline declarations

//boost include
#include <boost/format.hpp>
#include <boost/optional.hpp>

//stl includes
#include <cstdint>               //for int64_t and uint64_t
#include <string>

//gstreamer include
#include <gst/gstelement.h>
#include <gst/gstutils.h>        //for gst_element_link


namespace avs { namespace gstreamer {



class Element : public GstElement
{

private:  //declared but not implemented

  Element();
  ~Element();


public:  //access to pads

  Pad * GetPad(char const * name);


public: //access to bus

  PBus GetBus();


public:  //link

  bool Link(Element& target) { return gst_element_link(this, &target) != 0; }


public:  //seek method

  bool Seek(uint64_t time) {
    boost::format fmt = boost::format( "time (seek) : %1" ) % time;
    g_message ( fmt.str().c_str () );
    return gst_element_seek( this,
                                                    1.0,
                                                    GST_FORMAT_TIME,
                                                    (GstSeekFlags)(GST_SEEK_FLAG_ACCURATE | GST_SEEK_FLAG_FLUSH),
                                                    GST_SEEK_TYPE_CUR,
                                                    time,
                                                    GST_SEEK_TYPE_NONE,
                                                    -1) != 0; }


public:  //state stuff

  void SetStateNull() { ChangeState( GST_STATE_NULL ); }
  void SetStateReady() { ChangeState( GST_STATE_READY ); }
  void SetStatePaused() { ChangeState( GST_STATE_PAUSED ); }
  void SetStatePlaying() { ChangeState( GST_STATE_PLAYING ); }
  void SetSimpleStatePaused() { gst_element_set_state( this, GST_STATE_PAUSED ); }


public:

  int64_t QueryTotal();


public:  //casts

  operator Object&();

  Pipeline& AsPipeline();


public:  //factory method

  static Element * Create(char const * type, char const * name);


private:  //private methods

  void ChangeState(GstState state);

  boost::optional<std::string> GetErrorMessage();

};



} } //namespace avs::gstreamer

#endif //__AVS_GSTREAMER_ELEMENT_H__
