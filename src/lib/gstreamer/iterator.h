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


#ifndef __AVS_GSTREAMER_ITERATOR_H__
#define __AVS_GSTREAMER_ITERATOR_H__

//avisynth include
#include "forward.h"             //for Element, Pad declarations

//gstreamer include
#include <gst/gstiterator.h>
#include <gst/gstelement.h>        //for gst_element_iterate_src_pads


namespace avs { namespace gstreamer {



class Iterator
{

  GstIterator *iterator_;
  Pad *pad_;

public:  //declared but not implemented

  Iterator(GstIterator *iterator) { iterator_ = iterator; }
  ~Iterator() { gst_iterator_free( iterator_ ); }


public:  //iterate method

  bool Next();


public:  //access

  GstIterator * GetIterator() { return iterator_; }

  Pad * GetPad() { return pad_; }


public:  //factory method

  void SetIterator(GstIterator *iterator);

  static PIterator Create(Element& element);

};



} } //namespace avs::gstreamer

#endif //__AVS_GSTREAMER_ITERATOR_H__
