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
#include "bin.h"
#include "element.h"
#include "pipeline.h"

//stlport include
#include <cassert>


namespace avs { namespace gstreamer {



Pipeline::operator Bin&()
{
  return static_cast<Bin&>( *GST_BIN(this) );
}

Pipeline::operator Element&()
{
  return static_cast<Element&>( *GST_ELEMENT(this) );
}


namespace {


struct PipelineDestructor
{
  void operator()(Pipeline * pipeline) { }
};


} //namespace anonymous


std::shared_ptr<Pipeline> Pipeline::Create()
{
  Element * pipeline = static_cast<Element *>( gst_pipeline_new(NULL) );
  assert( pipeline != NULL );

  return std::shared_ptr<Pipeline>( &pipeline->AsPipeline(), PipelineDestructor() );
}



} } //namespace avs::gstreamer

#endif //AVS_HAS_GSTREAMER_SOURCE
