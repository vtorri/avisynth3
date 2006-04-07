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
#include "../../config.h"

#ifdef AVS_HAS_GSTREAMER_SOURCE

//avisynth includes
#include "gstreamersource.h"
#include "gstreamer/factory.h"
#include "gstreamer/pipeline.h"
#include "gstreamer/ownedholder.h"
#include "../../core/clip.h"
#include "../../core/forward.h"
#include "../../core/videoinfo.h"
#include "../../core/block/base.h"
#include "../../gstreamer/bin.h"
#include "../../gstreamer/buffer.h"
#include "../../gstreamer/object.h"
#include "../../gstreamer/element.h"
#include "../../core/colorspace/importer.h"


namespace avs { namespace filters {



GstreamerSource::GstreamerSource(source::gstreamer::Factory const& factory, PEnvironment const& env)
  : clip::framemaker::Concrete( env )
  , vi_( factory.vi() )
  , importer_( factory.importer() )
  , pipelineVideo_( factory.pipelineVideo() )
  , pipelineAudio_( factory.pipelineAudio() ) { }


CPVideoFrame GstreamerSource::operator()(long int n) const
{
  GstBuffer *buffer = NULL;
  avs::gstreamer::Object * sink = pipelineVideo_->GetSink();

  //seeking to the nth frame
  g_print ("we seek... %ld\n", n);
  g_signal_emit_by_name (G_OBJECT (sink), "seek", n, &buffer);

  return importer_->CreateFrame(vi_->GetDimension(),
				PROGRESSIVE,
                                owned_block<1>( new
                                                source::gstreamer::OwnedHolder(GetEnvironment(), *buffer)),
                                0 );
}

BYTE * GstreamerSource::GetAudio(BYTE * buffer, long long start, long count) const
{
  return NULL;
}


PClip GstreamerSource::Create(std::string const& fileName, int videoIndex, int audioIndex, PEnvironment const& env) 
{ 
  source::gstreamer::Factory factory(fileName, videoIndex, audioIndex);
  
  return PClip( static_cast<Clip *>(new GstreamerSource(factory, env)) ); 
}



} } //namespace avs::filters

#endif //AVS_HAS_GSTREAMER_SOURCE
