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
#include "structure.h"
#include "element.h"
#include "../core/exception/generic.h"

//stlport include
#include <cassert>

//gstreamer include
#include <gst/gstcaps.h>
#include <gst/gstutils.h>


namespace avs { namespace gstreamer {



namespace {


struct StructureDestructor
{
  GstCaps * caps_;

  StructureDestructor(GstCaps * caps) : caps_( caps ) { }

  void operator()(Structure * structure) const
  {
    gst_caps_unref( caps_ );
  }
};


} //namespace anonymous



PStructure Pad::GetStructure()
{
  GstCaps * caps = gst_pad_get_caps(this);

  return PStructure( static_cast<Structure *>(gst_caps_get_structure(caps, 0)), StructureDestructor(caps) );
}


void Pad::Link(Pad& sink)
{
  if ( GST_PAD_LINK_FAILED( gst_pad_link(this, &sink) ) )
    throw exception::Generic( "Gstreamer: can not link pads" );
}


int64 Pad::QueryTotal()
{
  GstQuery *query;

  query = gst_query_new_duration( GST_FORMAT_TIME );
  if ( !gst_pad_query( this, query ) )
    throw exception::Generic( "Gstreamer: can not query the length of the video pipeline" );
  int64 duration;
  gst_query_parse_duration( query, NULL, &duration );
  gst_query_unref( query );

  return duration;
}


bool Pad::CheckStream(char *stream_type)
{
  GstCaps * caps = gst_pad_get_caps(this);
  char * str = gst_caps_to_string( caps );
  bool res = g_str_has_prefix( str, stream_type ) != 0;
  g_free( str );
  gst_caps_unref( caps );

  return res;
}


Element& Pad::GetPeerElement()
{
  GstPad *pad = gst_pad_get_peer( this );
  return *static_cast<Element *>( gst_pad_get_parent_element( pad ) );
}



} } //namespace avs::gstreamer

#endif //AVS_HAS_GSTREAMER_SOURCE
