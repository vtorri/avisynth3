// Avisynth v3.0 alpha.  Copyright 2004 David Pierre - Ben Rudiak-Gould et al.
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
#include "glyph.h"
#include "../core/exception/generic.h"

// freetype includes
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

//assert include
//#include <assert.h>


namespace avs { namespace freetype {


Glyph::Glyph(FT_GlyphRec_ * glyph)
  : glyph_( glyph, &FT_Done_Glyph ) { }


Vecteur Glyph::GetAdvance() const
{
  return Vecteur( glyph_->advance.x, glyph_->advance.y );
}


Box Glyph::GetControlBox() const
{
  FT_BBox box;
  
}

/*  // Basic API
  
  void Glyph::render (FT_GlyphSlot    slot,
		      FT_Render_Mode render_mode)
  {
    FT_Error error;
    
    error = FT_Render_Glyph (slot, render_mode);
    
    if (error)
      throw avs::exception::Generic ("Error while rendering the Glyph.");
  }
  
    

  void Glyph::transform (FT_Matrix *matrix, FT_Vector *delta)
  {
    FT_Error error;
    
    error = FT_Glyph_Transform (glyph, matrix, delta);
    
    if (error)
      throw avs::exception::Generic ("Error while transforming the Glyph.");
  }

  void Glyph::get_cbox (FT_UInt bbox_mode, FT_BBox *cbox )
  {
    FT_Glyph_Get_CBox (glyph, bbox_mode, cbox);
  }

  void Glyph::convert_to_bitmap (FT_Render_Mode render_mode,
				 FT_Vector     *origin,
				 FT_Bool        destroy)
  {
    FT_Error error;
    
    error = FT_Glyph_To_Bitmap (&glyph, render_mode,
				origin, destroy);
    
    if (error)
      throw avs::exception::Generic ("Error while converting the Glyph to a Bitmap.");
  }*/

} } //namespace avs::freetype
