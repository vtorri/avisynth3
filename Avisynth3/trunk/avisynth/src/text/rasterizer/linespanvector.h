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


#ifndef __AVS_TEXT_RASTERIZER_LINESPANVECTOR_H__
#define __AVS_TEXT_RASTERIZER_LINESPANVECTOR_H__

//avisynth includes
#include "forward.h"               //for FlipVectorPair
#include "linespan.h"

//stl include
#include <vector>


namespace avs { namespace text { namespace rasterizer {



//////////////////////////////////////////////////////////////////////////////////////////////////
//  LineSpanVector
//
//  maintains an ordered list of non overlapping LineSpan
//  used to represent all lit pixels in a scan line
//  
class LineSpanVector
{

  typedef std::vector<LineSpan> SpanVector;

  SpanVector spanVector_;


public:  //structors

  LineSpanVector() { }
  LineSpanVector(FlipVectorPair const& flips);

  //generated copy constructor and destructor are fine


public:  //read access

  typedef SpanVector::const_iterator iterator;

  iterator begin() const { return spanVector_.begin(); }
  iterator end() const { return spanVector_.end(); }

  bool empty() const { return spanVector_.empty(); }


public:  

  void MergeThickened(LineSpanVector const& other, long radius);

};



} } } //namespace avs::text::rasterizer

#endif //__AVS_TEXT_RASTERIZER_LINESPANVECTOR_H__
