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
#include "library.h"
#include "face.h"
#include "../../core/exception/generic.h"

// freetype includes
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

//assert include
#include <assert.h>


namespace avs { namespace text { namespace freetype {



PFace Face::Create (std::string const& fileName, int index)
{
  FT_Face  face;
  FT_Error error = FT_New_Face(Library::instance, fileName.c_str(), index, &face);

  if ( error != 0 )
    throw avs::exception::Generic ("Error while creating a Face.");

  return PFace( static_cast<Face *>(face), &FT_Done_Face );
}



bool Face::HasKerning() const
{
  return FT_HAS_KERNING(const_cast<Face *>(this)) != 0;
}


VecteurFP6 Face::GetKerning(unsigned leftGlyph, unsigned rightGlyph) const
{
  FT_Vector result;
  FT_Error error = FT_Get_Kerning(const_cast<Face *>(this),
				  leftGlyph, rightGlyph,
				  FT_KERNING_DEFAULT, &result);
  
  if ( error != 0 )
    throw avs::exception::Generic ("Error while retrieving the kerning.");
  
  return VecteurFP6(result.x, result.y);
}


unsigned Face::GetCharIndex(unsigned charCode) const
{
  return FT_Get_Char_Index(const_cast<Face *>(this), charCode);
}


void Face::SetCharSize(VecteurFP6 const& size, DimensionFP6 const& resolution)  
{
  // FIXME: is this correct ?
  FT_Error error = FT_Set_Char_Size(this,
				    size.x.get(),
				    size.y.get(),
				    resolution.GetWidth().get(),
				    resolution.GetHeight().get());
  
  if ( error != 0 )
    throw avs::exception::Generic ("Error while setting a character dimensions.");
}


} } } //namespace avs::text::freetype
