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

//avisynth includes
#include "factory.h"
#include "pipeline.h"
#include "../../../gstreamer/pad.h"
#include "../../../gstreamer/element.h"

//gstreamer includes
#include <gst/gst.h>

//assert include
#include <assert.h>


namespace avs { namespace filters { namespace source { namespace gstreamer {

// struct PipelineDeleter
// {
//   void operator()(Pipeline *pipeline) const
//   {
//     g_print ("On delete\n");
//     pipeline->SetStateNull ();
//     gst_object_unref (GST_OBJECT (pipeline));
//   }
// };



PPipeline Pipeline::Create(std::string const& name)
{
  GstElement * pipeline = gst_pipeline_new("pipeline");
  assert( pipeline != NULL );

  // file source  
  Element * fileSrc = Element::FactoryMake("filesrc", "disk_source");

  static_cast<Object&>(*fileSrc).Set("location", name.c_str());

  // decodebin
  // FIXME: the gstreamer decoder has issues with mpeg streams
  //        must check for updates / API changes
  Element * decoder = Element::FactoryMake("decodebin", "decoder");

  Element * vsink = Element::FactoryMake("fakesink", "vsink");
  Element * asink = Element::FactoryMake("fakesink", "asink");

  // link and add elements in the pipeline
  gst_element_link (filesrc, decoder);
  gst_bin_add_many (GST_BIN (pipeline),
		    filesrc, decoder, vsink, asink, NULL);

  gst_bin_sync_children_state( GST_BIN(pipeline) );

  return PPipeline( static_cast<Pipeline *>(GST_PIPELINE (pipeline)) );
}


Pipeline::~Pipeline()
{
  g_print ("FIN\n");
  SetStateNull();
  gst_object_unref (GST_OBJECT (this));
}


GObject& Pipeline::GetDecoder ()
{
  return *G_OBJECT( gst_bin_get_by_name(GST_BIN(this), "decoder") );
}

GstElement& Pipeline::GetVideoSink()
{
  return *gst_bin_get_by_name(GST_BIN(this), "vsink");
}

GstElement& Pipeline::GetAudioSink()
{
  return *gst_bin_get_by_name(GST_BIN(this), "asink");
}


void Pipeline::SetStateNull()
{
  gst_element_set_state( GST_ELEMENT(this), GST_STATE_NULL );
}

void Pipeline::SetStateReady()
{
  gst_element_set_state( GST_ELEMENT(this), GST_STATE_READY );
}

void Pipeline::SetStatePaused()
{
  gst_element_set_state( GST_ELEMENT(this), GST_STATE_PAUSED );
}

void Pipeline::SetStatePlaying()
{
  gst_element_set_state( GST_ELEMENT(this), GST_STATE_PLAYING );
}


bool Pipeline::Iterate()
{
  return gst_bin_iterate( GST_BIN(this) ) != 0;
}

/*
void Pipeline::CreateVideoSink()
{
  GstElement *videobin = gst_bin_new ("videobin");
  GstElement *vcs = gst_element_factory_make ("ffmpegcolorspace", NULL);
  GstElement *vsink = gst_element_factory_make ("fakesink", "vsink");

  gst_bin_add_many (GST_BIN (videobin),
		    vcs, vsink, NULL);
  gst_element_link_many (vcs, vsink, NULL);
  gst_element_add_ghost_pad (videobin,
			     gst_element_get_pad (vcs, "sink"), "sink");
  gst_bin_add (GST_BIN (this), videobin);
}
  
void Pipeline::CreateAudioSink()
{
  GstElement *audiobin = gst_bin_new ("audiobin");
  GstElement *conv = gst_element_factory_make ("audioconvert", NULL);
  GstElement *scale = gst_element_factory_make ("audioscale", NULL);
  GstElement *asink = gst_element_factory_make ("fakesink", "asink");

  gst_bin_add_many (GST_BIN (audiobin),
		    conv, scale, asink, NULL);
  gst_element_link_many (conv, scale, asink, NULL);
  gst_element_add_ghost_pad (audiobin,
			     gst_element_get_pad (conv, "sink"), "sink");
  
  gst_bin_add (GST_BIN (this), audiobin);
}*/


void Pipeline::GoToFrame (int frame_number, Fraction& fps)
{
  GstElement *vsink = gst_bin_get_by_name (GST_BIN (this), "vsink");
  GstElement *asink = gst_bin_get_by_name (GST_BIN (this), "asink");
  GstSeekType type = (GstSeekType)(GST_FORMAT_TIME     |
				   GST_SEEK_METHOD_SET |
				   GST_SEEK_FLAG_FLUSH);
  long long int time = 1000000000*frame_number*fps.numerator() / fps.denominator();
  if (vsink)
    gst_element_seek (vsink, type, time);
  if (asink)
    gst_element_seek (asink, type, time);
}

int Pipeline::QueryVideoLength(Fraction& fps)
{
  GstFormat   fmt = GST_FORMAT_TIME;
  GstElement *vsink = gst_bin_get_by_name (GST_BIN (this), "vsink");
  gint64      length;

  if (vsink && gst_element_query (vsink, GST_QUERY_TOTAL, &fmt, &length)) {
    fmt = GST_FORMAT_DEFAULT;
    if (!gst_element_query (vsink, GST_QUERY_TOTAL,
			    &fmt, &length))
      length = (fps.numerator() * length) / (fps.denominator() * 1000000000);
    return length;
  }
}

int Pipeline::QueryAudioLength(int samplerate)
{
  GstFormat   fmt = GST_FORMAT_TIME;
  GstElement *asink = gst_bin_get_by_name (GST_BIN (this), "asink");
  gint64      length;

  if (asink && gst_element_query (asink, GST_QUERY_TOTAL, &fmt, &length)) {
    fmt = GST_FORMAT_DEFAULT;
    if (!gst_element_query (asink, GST_QUERY_TOTAL, &fmt, &length))
      length = (samplerate * length) / 1000000000;
    return length;
  }
}

} } } } // namespace avs::filters::source::gstreamer
