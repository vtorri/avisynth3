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


//avisynth includes
#include "outline.h"
#include "library.h"
#include "../rasterizer/outlinesplitter.h"

//assert include
#include <assert.h>


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


void Outline::Split(rasterizer::OutlineSplitter& splitter, VecteurFP6 const& pen_) const
{
  typedef rasterizer::VecteurFP3 VecteurFP3;
  VecteurFP3 const& pen = reinterpret_cast<VecteurFP3 const&>(pen_);

  VecteurFP3 const * ptIt = reinterpret_cast<VecteurFP3 *>(points);
  char * tagPtIt = tags;
  short * contourEndIt = contours;

  for ( int i = n_contours; i-- > 0; )      //loop over the outline contours
  {
    //past the end end position for the list of points of the current contour
    VecteurFP3 const * ptItEnd = reinterpret_cast<VecteurFP3 *>(points) + *contourEndIt + 1;

    splitter.StartContour(pen + *ptIt++);
    ++tagPtIt;

    while ( ptIt < ptItEnd )
    {
      VecteurFP3 const& pt1 = *ptIt++;
      if ( (*tagPtIt++ & 1) == 0 )            //if not a control point
        splitter.LineTo( pt1 );               //just a line
      else
      {
        VecteurFP3 const& pt2 = *ptIt++;      //take a second pt
        if ( (*tagPtIt++ & 1) == 0 )          //if not a control point
          splitter.BezierCurveTo(pt2, pt1);   //2nr order Bezier
        else
        {
          splitter.BezierCurveTo( *ptIt++, pt2, pt1 );   //3rd order Bezier
          ++tagPtIt;
        }
      }
    }

    splitter.CloseContour();
  }
}


} } } //namespace avs::text::freetype
