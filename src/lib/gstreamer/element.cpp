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


//macro include
#include "../config.h"

#ifdef AVS_HAS_GSTREAMER_SOURCE

//avisynth includes
#include "pad.h"
#include "object.h"
#include "element.h"
#include "bus.h"
#include "message.h"
#include "pipeline.h"
#include "../core/exception/gstreamer.h"

//stlport include
#include <cassert>


namespace avs { namespace gstreamer {



struct BusDestructor
{
  void operator()(Bus * bus) const
  {
    gst_object_unref ( GST_OBJECT ( bus ) );
  }
};


Pad * Element::GetPad(char const * name)
{
  return static_cast<Pad *>( gst_element_get_pad(this, name) );
}


PBus Element::GetBus()
{
  Bus * bus = static_cast<Bus *>( gst_element_get_bus( this ) );
  assert( bus != NULL );

  return std::shared_ptr<Bus>( bus, BusDestructor() );
}


int64_t Element::QueryTotal()
{
  GstQuery *query;

  query = gst_query_new_duration( GST_FORMAT_TIME );
  if ( !gst_pad_query( GetPad( "sink" ), query ) )
    throw exception::Gstreamer( "Can not query the length of the video pipeline" );
  int64_t duration;
  gst_query_parse_duration( query, NULL, &duration );
  gst_query_unref( query );

  return duration;
}


Element::operator Object&()
{
  return *static_cast<Object *>( G_OBJECT(this) );
}

Pipeline& Element::AsPipeline()
{
  assert( GST_IS_PIPELINE(this) != 0 );

  return *static_cast<Pipeline *>( GST_PIPELINE(this) );
}


Element * Element::Create(char const * type, char const * name)
{
  GstElement * result = gst_element_factory_make(type, name);
  assert( result != NULL );
  return static_cast<Element *>(result);
}


void Element::ChangeState(GstState state)
{
  GstStateChangeReturn res = gst_element_set_state( this, state );
  if (res == GST_STATE_CHANGE_FAILURE) {
    std::string msg = "Can not pause the pipeline";
    boost::optional<std::string> str = GetErrorMessage();
    if (str) msg += "\n" + *str;
    throw exception::Gstreamer( msg );
  }
  res = gst_element_get_state( this, NULL, NULL, GST_CLOCK_TIME_NONE );
  if (res != GST_STATE_CHANGE_SUCCESS)
    throw exception::Gstreamer( "Can not complete pause state of the pipeline" );
}


boost::optional<std::string> Element::GetErrorMessage()
{
  PBus bus = GetBus();
  bool done = false;
  boost::optional<std::string> str;

  while (!done) {
    PMessage msg = bus->GetMessage();

    switch (msg->GetType()) {
    case GST_MESSAGE_ERROR: {
       str = msg->GetError();
      if (str)
	str = "Reason: " + str->substr( str->find_first_of( '\n' ) + 1 );
      done = true;
      break;
    }
    default:
      break;
    }
  }

  return str;
}



} } //namespace avs::gstreamer

#endif //AVS_HAS_GSTREAMER_SOURCE
