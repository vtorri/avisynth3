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


//avisynth include
#include "face.h"
#include "outline.h"
#include "glyphslot.h"

// freetype includes
#include <ft2build.h>
#include FT_FREETYPE_H

//assert include
#include <assert.h>


namespace avs { namespace text { namespace freetype {



GlyphSlot::GlyphSlot(PFace const& face)
  : face_( face )
  , glyphslot_( face->glyph ) { }


void GlyphSlot::LoadGlyph (unsigned int index)
{
  FT_Load_Glyph(face_.get(), index, FT_LOAD_RENDER | FT_LOAD_TARGET_MONO);
}


VecteurFP6 GlyphSlot::GetAdvance() const
{
  return VecteurFP6( glyphslot_->advance.x, glyphslot_->advance.y);
}


Outline const& GlyphSlot::GetOutline() const
{
  return static_cast<Outline&>(glyphslot_->outline);
}


} } } //namespace avs::text::freetype
