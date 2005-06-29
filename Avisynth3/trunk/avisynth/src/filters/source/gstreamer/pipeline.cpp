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


#ifndef _WIN32

//avisynth includes
#include "factory.h"
#include "pipeline.h"
#include "../../../core/videoinfo.h"
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

  avs::gstreamer::Bin& binPipeline = pipeline->operator avs::gstreamer::Bin&();

  //file source  
  avs::gstreamer::Element& fileSrc = binPipeline.AddNewElement("filesrc", "disk_source");
  fileSrc.operator avs::gstreamer::Object&().Set("location", name.c_str());

  //links fileSrc to a decoder
  fileSrc.Link( binPipeline.AddNewElement("decodebin", "decoder") );

  // FIXME: the gstreamer decoder has issues with mpeg streams
  //        must check for updates / API changes  

  binPipeline.AddNewElement("fakesink", "vsink");
  binPipeline.AddNewElement("fakesink", "asink");

  binPipeline.SyncChildrenState();

  return pipeline;
}


avs::gstreamer::Element& Pipeline::GetDecoder() { return *operator avs::gstreamer::Bin&().GetByName("decoder"); }
avs::gstreamer::Element& Pipeline::GetVideoSink() { return *operator avs::gstreamer::Bin&().GetByName("vsink"); }
avs::gstreamer::Element& Pipeline::GetAudioSink() { return *operator avs::gstreamer::Bin&().GetByName("asink"); }


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


namespace {


bool SetFrameCount(VideoInfo& vi, avs::gstreamer::Element& sink)
{
  long long length;

  if ( ! sink.QueryTotal(GST_FORMAT_DEFAULT, length) )
    if ( ! sink.QueryTotal(GST_FORMAT_TIME, length) )
      return true;
    else
    {
      length *= vi.GetFPSNumerator();
      length /= vi.GetFPSDenominator() * 1000000000;
    }
           
  vi.SetFrameCount( static_cast<int>(length) );
  return false;
}


bool SetSampleCount(VideoInfo& vi, avs::gstreamer::Element& sink)
{
  long long length;

  if ( ! sink.QueryTotal(GST_FORMAT_DEFAULT, length) )
    if ( ! sink.QueryTotal(GST_FORMAT_TIME, length) )
      return true;
    else
      length = length * vi.GetSampleRate() / 1000000000;

  vi.SetSampleCount(length);
  return false;
}


} //namespace anonymous



void Pipeline::SetLengths(VideoInfo& vi)
{
  avs::gstreamer::Bin& bin = *this;
  avs::gstreamer::Element& vsink = GetVideoSink();
  avs::gstreamer::Element& asink = GetAudioSink();

  int i = 30;
  bool handleVideo = vi.HasVideo();
  bool handleAudio = vi.HasAudio();

  do
  {
    if ( handleVideo )
      handleVideo = SetFrameCount(vi, vsink);
    if ( handleAudio )
      handleAudio = SetSampleCount(vi, asink);
  }
  while( i-- > 0 && (handleVideo || handleAudio) && bin.Iterate() );

  //throw if handleVideo or handleAudio is still true !?
}



} } } } // namespace avs::filters::source::gstreamer

#endif //_WIN32
