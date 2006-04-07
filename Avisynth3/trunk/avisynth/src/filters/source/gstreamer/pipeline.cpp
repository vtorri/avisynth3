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


//macro include
#include "../../../config.h"

#ifdef AVS_HAS_GSTREAMER_SOURCE

//avisynth includes
#include "factory.h"
#include "pipeline.h"
#include "../../../core/videoinfo.h"
#include "../../../gstreamer/bin.h"
#include "../../../gstreamer/pad.h"
#include "../../../gstreamer/iterator.h"


namespace avs { namespace filters { namespace source { namespace gstreamer {



namespace {


static void DetectPadsCallback(GObject * obj, GstPad * pad, gboolean last, void * data)
{
  static_cast<Pipeline *>(data)->PadDetected( static_cast<avs::gstreamer::Pad&>(*pad) );
}


} // anonymous namespace

struct PipelineDestructor
{
  void operator()(Pipeline *pipeline) const
  {
    g_print ("On delete la pipeline\n");
    avs::gstreamer::Element& elementPipeline = pipeline->GetPipeline();
    avs::gstreamer::Object& objectPipeline = elementPipeline.operator avs::gstreamer::Object&();
    elementPipeline.SetStateNull ();
    gst_object_unref (GST_OBJECT (&objectPipeline));
  }
};


Pipeline::Pipeline(std::string const& filename)
  : pipeline_( BuildPipeline( filename ) )
  , detectPad_( &GetDecoder().operator avs::gstreamer::Object&(), "new-decoded-pad", &DetectPadsCallback, this )
  , pad_( NULL )
  , sink_( NULL ) { }


avs::gstreamer::PPipeline Pipeline::BuildPipeline(std::string const& filename)
{
  avs::gstreamer::PPipeline pipeline = avs::gstreamer::Pipeline::Create();

  avs::gstreamer::Bin& binPipeline = pipeline->operator avs::gstreamer::Bin&();

  //file source
  avs::gstreamer::Element& fileSrc = binPipeline.AddNewElement( "filesrc", "disk_source" );
  fileSrc.operator avs::gstreamer::Object&().Set( "location", filename.c_str() );

  //decodebin
  avs::gstreamer::Element& decoder = binPipeline.AddNewElement( "decodebin", "decoder" );

  //links fileSrc to the decoder
  fileSrc.Link( decoder );

  return pipeline;  
}


PPipeline Pipeline::Create(std::string const& filename)
{
  g_print ("Pipeline Create\n");
  PPipeline pipeline = boost::shared_ptr<Pipeline>( new Pipeline( filename ), PipelineDestructor() );

  return pipeline;
}


avs::gstreamer::Element& Pipeline::GetPipeline() { return pipeline_->operator avs::gstreamer::Element&(); }
avs::gstreamer::Element& Pipeline::GetDecoder() { return *pipeline_->operator avs::gstreamer::Bin&().GetByName("decoder"); }


void Pipeline::SetStatePaused()
{
  avs::gstreamer::Element& elementPipeline = *pipeline_;
  elementPipeline.SetStatePaused();
}


void Pipeline::PadDetected(avs::gstreamer::Pad& pad)
{
  GstCaps *caps = gst_pad_get_caps (&pad);
  g_print ("new pad %s\n\n", gst_caps_to_string (caps));
  avs::gstreamer::Bin& binPipeline = pipeline_->operator avs::gstreamer::Bin&();
  avs::gstreamer::Element& sink = binPipeline.AddNewElement( "avs3sink", NULL );
  pad.Link( *sink.GetPad( "sink" ) );
  sink.SetSimpleStatePaused();
}

void Pipeline::SetSink (int index, char *stream_type)
{
  avs::gstreamer::Element& decoder = GetDecoder();
  avs::gstreamer::PIterator iter = avs::gstreamer::Iterator::Create(decoder);
  int i = 0;
  while ( iter->Next() ) {
    avs::gstreamer::Pad *pad = iter->GetPad();
    if (pad->CheckStream(stream_type)) {
      if (i == index) {
        pad_ = pad;
        sink_ = &pad->GetPeerElement();
        break;
      }
      i++;
    }
  }
}


void Pipeline::SetFrameCount(VideoInfo& vi)
{
  int64 length = pad_->QueryTotal();

  vi.SetFrameCount( static_cast<int>( ( length * vi.GetFPSNumerator() ) / ( vi.GetFPSDenominator() * GST_SECOND ) ) );
}


void Pipeline::SetSampleCount(VideoInfo& vi)
{
  int64 length = pad_->QueryTotal();

  vi.SetSampleCount( static_cast<long long>( length * vi.GetSampleRate() / GST_SECOND ) );
}



} } } } //namespace avs::filters::source::gstreamer

#endif //AVS_HAS_GSTREAMER_SOURCE
