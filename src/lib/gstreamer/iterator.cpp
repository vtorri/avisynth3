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
#include "../config.h"

#ifdef AVS_HAS_GSTREAMER_SOURCE

//avisynth includes
#include "pad.h"
#include "iterator.h"
#include "element.h"

// FIXME: necessary ?
//stl include
#include <memory>


namespace avs { namespace gstreamer {



namespace {


struct IteratorDestructor
{
  void operator()(Iterator * iter) { gst_iterator_free( iter->GetIterator() ); }
};


} //namespace anonymous



bool Iterator::Next()
{
  gpointer data;
  int res = gst_iterator_next( iterator_, &data);
  pad_ = static_cast<Pad *>(data);

  return res == GST_ITERATOR_OK;
}


void Iterator::SetIterator(GstIterator *iter)
{
  iterator_ = iter;
}


PIterator Iterator::Create(Element& element)
{
  GstIterator *iter = gst_element_iterate_src_pads( &element);
  return PIterator( new Iterator(iter) );
}



} } //namespace avs::gstreamer

#endif //AVS_HAS_GSTREAMER_SOURCE
