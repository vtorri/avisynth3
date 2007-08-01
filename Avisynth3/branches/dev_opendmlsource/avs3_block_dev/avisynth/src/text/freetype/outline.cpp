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


//avisynth includes
#include "outline.h"
#include "library.h"
#include "../rasterizer/outlinesplitter.h"

//stlport include
#include <cassert>

//freetype include
#include FT_IMAGE_H


namespace avs { namespace text { namespace freetype {



Outline::Outline(Outline const& other)
{
  FT_Error error = FT_Outline_New(Library::instance, other.n_points, other.n_contours, this);
  assert( error == 0 );
  
  error = FT_Outline_Copy(const_cast<Outline *>(&other), this);
  assert( error == 0 );
}


Outline::~Outline()
{
  FT_Error error = FT_Outline_Done(Library::instance, this);
    
  assert( error == 0 );
}



void Outline::Translate(VecteurFP6 const& shift)
{
  FT_Outline_Translate(const_cast<Outline *>(this), shift.x.get(), shift.y.get());
}


BoxFP6 Outline::GetControlBox() const
{
  FT_BBox box;
  FT_Outline_Get_CBox(const_cast<Outline *>(this), &box);        //can't fail: returns void

  return BoxFP6(VecteurFP6(FP6::Wrap(box.xMin), FP6::Wrap(box.yMin)), VecteurFP6(FP6::Wrap(box.xMax), FP6::Wrap(box.yMax)));
}


using namespace rasterizer;

namespace {

VecteurFP3 Wrap(FT_Vector const& v) { return VecteurFP3( FP3::Wrap(v.x), FP3::Wrap(v.y) ); }

} //namespace anonymous


void Outline::Split(rasterizer::OutlineSplitter& splitter, VecteurFP6 const& pen_) const
{
  VecteurFP3 pen = VecteurFP3( FP3::Wrap(pen_.x.get()), FP3::Wrap(pen_.y.get()) );

  FT_Vector const * ptIt = points;
  char * tagPtIt = tags;
  short * contourEndIt = contours;
  
  bool clockWiseFill = (flags & FT_OUTLINE_REVERSE_FILL) == 0;

  //loop over the outline contours
  for ( int i = n_contours; i-- > 0; ++contourEndIt, ++ptIt, ++tagPtIt )
  {
    FT_Vector const * ptItLast = points + *contourEndIt;

    splitter.StartContour(pen + Wrap(*ptIt), clockWiseFill);

    while ( true )
    {
      if ( ptIt == ptItLast )                      //if current pt (on curve) is the last one
      {
        splitter.CloseContour();                   //close with a line
        break;
      }

      ++ptIt;
      ++tagPtIt;                                   //move to next pt, exists since previous is not last
      VecteurFP3 pt1 = pen + Wrap(*ptIt);          //take it
      
      if ( (*tagPtIt & 1) == FT_Curve_Tag_On )     //if on the curve
      {
        splitter.LineTo( pt1 );                    //just a line
        continue;
      }

      if ( ptIt == ptItLast )                      //if our control pt is the last pt
      {
        splitter.BezierClose(pt1);                 //close with a 2nd order bezier
        break;
      }
      
      ++ptIt;
      ++tagPtIt;                                   //move to next pt  ... 
      VecteurFP3 pt2 = pen + Wrap(*ptIt);          //and take it
      
      if ( (*tagPtIt & 1) == FT_Curve_Tag_On )     //if on the curve
      {
        splitter.BezierCurveTo(pt2, pt1);          //2nd order Bezier
        continue;
      }

      if ( ptIt == ptItLast )                      //if our 2nd control pt is the last pt
      {
        if ( (*tagPtIt & 2) == FT_Curve_Tag_Cubic )   //and 3rd order   
          splitter.BezierClose(pt2, pt1);             //close with 3rd order Bezier
        else
        {
          splitter.BezierCurveTo( (pt1 +pt2) / 2, pt1 );
          splitter.BezierClose(pt2);                  //close with two 2nd order Bezier
        }
        break;
      }

      ++ptIt;                                       //move to next pt  ...
      VecteurFP3 pt3 = pen + Wrap(*ptIt);           //take a 3rd pt

      if ( (*tagPtIt & 2) == FT_Curve_Tag_Cubic )   //if previous pt is a 3rd order control pt
        splitter.BezierCurveTo( pt3, pt2, pt1 );    //3rd order Bezier
      else
      {
        splitter.BezierCurveTo( (pt1 + pt2) / 2, pt1 );
        splitter.BezierCurveTo( pt3, pt2 );         //two 2nd order Bezier
      }
      ++tagPtIt;                                    //resynch with ptIt
    }      
  }
}



} } } //namespace avs::text::freetype
