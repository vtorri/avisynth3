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


#ifndef __AVS_TEXT_RASTERIZER_LINESPAN_H__
#define __AVS_TEXT_RASTERIZER_LINESPAN_H__

//avisynth includes
#include "forward.h"             //for LineSpanVector
#include "../../core/forward.h"  //for BYTE

//stl include
#include <vector>                //for vector::const_iterator


namespace avs { namespace text { namespace rasterizer {



////////////////////////////////////////////////////////////////////////////////////////////////////////
//  LineSpan
//
//  represents a group of pixels lit in a line, starting at begin and fisnishing at end (not included)
//
class LineSpan
{
  
public:  //members

  long begin;
  long end;


public:  //structors

  LineSpan() { }
  LineSpan(long begin_, long end_) : begin( begin_ ), end( end_ ) { }

  //generated copy constructor and destructor are fine


public:

  //returns a thickened span (NB: not the concern of this to check radius >=0)
  LineSpan Thicken(long radius) const { return LineSpan(begin - radius, end + radius); }

  void Realize(BYTE * ptr, int step) const;


private:  //helpers for LineSpanVector

  typedef std::vector<LineSpan>::const_iterator iterator;

  bool RightMerge(iterator& beginIt, iterator endIt);
  bool RightMergeThickened(iterator& beginIt, iterator endIt, long radius);

  friend class LineSpanVector;    //so can call the above

};


//WEAK ordering on span, means left is strictly before right (no overlap)
inline bool operator<(LineSpan const& left, LineSpan const& right)
{
  return left.end <= right.begin;
}


inline bool operator<(long begin, LineSpan const& span) { return begin <= span.begin; }
inline bool operator<(LineSpan const& span, long end) { return span.end <= end; }

inline bool operator<=(LineSpan const& span, long end) { return span.begin < end; }
inline bool operator<=(long begin, LineSpan const& span) { return begin < span.end; }



} } } //namespace avs::text:rasterizer

#endif //__AVS_TEXT_RASTERIZER_LINESPAN_H__
