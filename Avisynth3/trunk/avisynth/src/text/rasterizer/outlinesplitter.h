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


#ifndef __AVS_TEXT_RASTERIZER_OUTLINESPLITTER_H__
#define __AVS_TEXT_RASTERIZER_OUTLINESPLITTER_H__

//avisynth include
#include "spanmaker.h"


namespace avs { namespace text { namespace rasterizer {



class OutlineSplitter
{

  SpanMaker spanMaker_;


public:

  void StartContour(VecteurFP3 const& pt) { spanMaker_.StartPolygon(pt); }

  void CloseContour() { spanMaker_.ClosePolygon(); }
  bool IsContourClosed() const { return spanMaker_.IsPolygonClosed(); }

  void LineTo(VecteurFP3 const& pt) { spanMaker_.LineTo(pt); }
  void BezierCurveTo(VecteurFP3 const& pt2, VecteurFP3 const& pt1);
  void BezierCurveTo(VecteurFP3 const& pt3, VecteurFP3 const& pt2, VecteurFP3 const& pt1);


private:

  typedef vecteur<double> VecteurDb;

  void InterpolatePolynom(VecteurDb const& A, VecteurDb const& B, VecteurDb const& C, VecteurFP3 const& D);
  
};



} } } //namespace avs::text::rasterizer

#endif //__AVS_TEXT_RASTERIZER_OUTLINESPLITTER_H__
