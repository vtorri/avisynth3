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
#include "monobitmap.h"

//assert include
#include <assert.h>


namespace avs { namespace text { namespace freetype {



namespace {


//custom shared_ptr deleter for Outline
struct OutlineDeleter
{
  void operator()(Outline * outline) const
  {
    FT_Error error = FT_Outline_Done (Library::instance, outline );
    
    assert( error == 0 );
  }
};


} //namespace anonymous


POutline Outline::clone() const
{
  FT_Outline * result = NULL;

  FT_Outline_New(Library::instance, n_points, n_contours, result);
  if ( result == NULL )
    throw std::bad_alloc();

  FT_Error error = FT_Outline_Copy(const_cast<Outline *>(this), result);
  assert( error == 0 );

  return POutline( static_cast<Outline *>(result), OutlineDeleter() );
}


void Outline::Translate(VecteurFP6 const& shift)
{
  FT_Outline_Translate(const_cast<Outline *>(this), shift.x.get(), shift.y.get());
}


BoxFP6 Outline::GetControlBox() const
{
  FT_BBox box;
  FT_Outline_Get_CBox(const_cast<Outline *>(this), &box);        //can't fail: returns void

  return BoxFP6(VecteurFP6(box.xMin, box.yMin), VecteurFP6(box.xMax, box.yMax));
}



void Outline::Draw(MonoBitmap const& bitmap) const
{
  FT_Error error = FT_Outline_Get_Bitmap(Library::instance, const_cast<Outline *>(this), const_cast<MonoBitmap *>(&bitmap));

  assert( error == 0 );
}



} } } //namespace avs::text::freetype
