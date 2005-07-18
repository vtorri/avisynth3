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
#include "gstreamersource.h"
#include "video/importer.h"
#include "gstreamer/factory.h"
#include "gstreamer/pipeline.h"
#include "gstreamer/ownedholder.h"
#include "../../core/clip.h"
#include "../../core/forward.h"
#include "../../core/videoinfo.h"
#include "../../core/block/base.h"



namespace avs { namespace filters {



GstreamerSource::GstreamerSource(source::gstreamer::Factory const& factory, PEnvironment const& env)
  : clip::framemaker::Concrete( env )
  , vi_( factory.vi() )
  , importer_( factory.importer() )
  , pipeline_( factory.pipeline() ) { }


CPVideoFrame GstreamerSource::MakeFrame(int n) const
{
  PVideoFrame frame;
  FrameType ft;
  Fraction fps = vi_->GetFPS();
  GstBuffer *buffer = NULL;

  //seeking to the nth frame
  pipeline_->GoToFrame (n, fps);
  
  //get the frame
  g_object_get (G_OBJECT (pipeline_.get()), "frame", &buffer, NULL);

  return importer_->CreateFrame(vi_->GetDimension(),
				owned_block<1>( new gstreamer::OwnedHolder(GetEnvironment(), *buffer) ));
}

} } //namespace avs::filters

#endif //_WIN32
