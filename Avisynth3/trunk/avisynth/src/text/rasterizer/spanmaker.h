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


#ifndef __AVS_TEXT_RASTERIZER_SPANMAKER_H__
#define __AVS_TEXT_RASTERIZER_SPANMAKER_H__

//avisynth includes
#include "forward.h"
#include "../../core/geometry/vecteur.h"
#include "../../core/geometry/fixed_point.h"

//stl include
#include <map>
#include <vector>


namespace avs { namespace text { namespace rasterizer {



////////////////////////////////////////////////////////////////////////////////////////
//  SpanMaker
//
//  process lines into a list of entering and exiting transitions (into a figure)
//  whose can later be changed into spans
//
//  NB: lines are defined using a superior precision (x8) than the spans
//
class SpanMaker
{

  typedef std::map<long, FlipVectorPair> FlipMap;

  FlipMap flipMap_;

  VecteurFP3 lastPt_;       //last point
  VecteurFP3 startPt_;      //starting point (of the current figure)


public:  //interface

  void StartPolygon(VecteurFP3 const& pt);

  void LineTo(VecteurFP3 const& pt);

  //ensure current polygon is closed
  void ClosePolygon();
  bool IsPolygonClosed() const { return lastPt_ == startPt_; }

  //fetchs last pt
  VecteurFP3 const& GetLastPt() const { return lastPt_; }


private:  //helpers for SpanMap constructor

  typedef FlipMap::const_iterator iterator;

  iterator begin() const { return flipMap_.begin(); }
  iterator end() const { return flipMap_.end(); }

  friend class SpanMap;   //so can use them

};



} } } //namespace avs::text::rasterizer

#endif //__AVS_TEXT_RASTERIZER_SPANMAKER_H__
