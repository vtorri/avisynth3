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
#include "../../../gstreamer/bin.h"
#include "../../../gstreamer/pad.h"
#include "../../../gstreamer/object.h"
#include "../../../gstreamer/element.h"

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
  PPipeline pipeline = boost::static_pointer_cast<Pipeline>( avs::gstreamer::Pipeline::Create() );

  avs::gstreamer::Bin& binPipeline = *pipeline;

  //file source  
  avs::gstreamer::Element& fileSrc = binPipeline.AddNewElement("filesrc", "disk_source");
  static_cast<avs::gstreamer::Object&>(fileSrc).Set("location", name.c_str());

  //links fileSrc to a decoder
  fileSrc.Link( binPipeline.AddNewElement("decodebin", "decoder") );

  // FIXME: the gstreamer decoder has issues with mpeg streams
  //        must check for updates / API changes  

  binPipeline.AddNewElement("fakesink", "vsink");
  binPipeline.AddNewElement("fakesink", "asink");

  binPipeline.SyncChildrenState();

  return pipeline;
}


avs::gstreamer::Element& Pipeline::GetDecoder() { return *operator Bin&().GetByName("decoder"); }
avs::gstreamer::Element& Pipeline::GetVideoSink() { return *operator Bin&().GetByName("vsink"); }
avs::gstreamer::Element& Pipeline::GetAudioSink() { return *operator Bin&().GetByName("asink"); }




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
  GstSeekType type = (GstSeekType)(GST_FORMAT_TIME     |
				   GST_SEEK_METHOD_SET |
				   GST_SEEK_FLAG_FLUSH);
  long long int time = 1000000000*frame_number*fps.numerator() / fps.denominator();

  //TODO: handle cases where there is no video or no audio
  GetVideoSink().Seek(type, time);
  GetAudioSink().Seek(type, time);
}

int Pipeline::QueryVideoLength(Fraction& fps)
{
  long long length;
  Element& vsink = GetVideoSink();

  if ( vsink.QueryTotal(GST_FORMAT_DEFAULT, length) )
    return length;

  vsink.QueryTotal(GST_FORMAT_TIME, length);

  return length * fps.numerator() / fps.denominator() / 1000000000;
}

int Pipeline::QueryAudioLength(int samplerate)
{
  long long length;
  Element & asink = GetAudioSink();

  if ( asink.QueryTotal(GST_FORMAT_DEFAULT, length) )
    return length;

  asink.QueryTotal(GST_FORMAT_TIME, length);
  
  return length * samplerate / 1000000000;
}


} } } } // namespace avs::filters::source::gstreamer
