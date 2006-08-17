// Avisynth v3.0 alpha.  Copyright 2005-2006 David Pierre - Ben Rudiak-Gould et al.
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
#include "pipeline.h"
#include "../core/exception/generic.h"

//stlport include
#include <cassert>


namespace avs { namespace gstreamer {



Pad * Element::GetPad(char const * name)
{ 
  return static_cast<Pad *>( gst_element_get_pad(this, name) ); 
}


int64 Element::QueryTotal()
{
  GstQuery *query;
      
  query = gst_query_new_duration( GST_FORMAT_TIME );
  if ( !gst_pad_query( GetPad( "sink" ), query ) )
    throw exception::Generic( "Gstreamer: can not query the length of the video pipeline" );
  int64 duration;
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
  if (res == GST_STATE_CHANGE_FAILURE)
    throw exception::Generic( "Gstreamer: can not pause" );
  res = gst_element_get_state( this, NULL, NULL, GST_CLOCK_TIME_NONE );
  if (res != GST_STATE_CHANGE_SUCCESS)
    throw exception::Generic( "Gstreamer: can not complete pause" );
}



} } //namespace avs::gstreamer

#endif //AVS_HAS_GSTREAMER_SOURCE
