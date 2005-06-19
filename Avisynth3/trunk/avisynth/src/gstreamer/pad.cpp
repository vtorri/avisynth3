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
#include "pad.h"
#include "structure.h"

//assert include
#include <assert.h>

//gstreamer include
#include <gst/gstcaps.h>


namespace avs { namespace gstreamer {


  
namespace {


struct StructureDestructor
{
  GstCaps * caps_;

  StructureDestructor(GstCaps * caps) : caps_( caps ) { }

  void operator()(Structure * structure) const
  {
    gst_free_caps(caps_);
  }
};


} //namespace anonymous



PStructure Pad::GetStructure()
{
  GstCaps * caps = gst_pad_get_caps(this);
  
  return PStructure( static_cast<Structure *>(gst_caps_get_structure(caps, 0)), StructureDestructor(caps) );
}


PStructure Pad::GetNegotiatedStructure()
{
  G_CONST_RETURN GstCaps * caps = gst_pad_get_negotiated_caps(this);

  return PStructure( static_cast<Structure *>(gst_caps_get_structure(caps, 0)), std::identity<void *>() );
}



} } //namespace avs::gstreamer

#endif //_WIN32
