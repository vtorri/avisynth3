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
#include "face.h"
#include "font.h"
#include "outline.h"
#include "glyphslot.h"


namespace avs { namespace text { namespace freetype {



Font::Font(std::string const& name, int size)
{
  face_ = Face::Create(name);
  face_->SetCharSize( VecteurFP6(0, size << 6), DimensionFP6(300, 300) );
}



BoxFP6 Font::GetTextBoundingBox(std::string const& text)
{
  int glyphCount = text.length();

  if ( glyphCount == 0 )   //if no text
    return BoxFP6();       //empty box

  GlyphSlot slot(face_);
  bool useKerning = face_->HasKerning();

  unsigned prevIndex = face_->GetCharIndex(text[0]);
  slot.LoadGlyph(prevIndex);

  VecteurFP6 pen = slot.GetAdvance();
  BoxFP6 box = slot.GetOutline()->GetControlBox();

  for ( int n = 1; n < glyphCount; ++n, prevIndex = glyphIndex )
  {
    unsigned glyphIndex = face_->GetCharIndex(text[n]);

    slot.LoadGlyph(glyphIndex);

    if ( useKerning )
      pen += face_->GetKerning(prevIndex, glyphIndex);

    box |= slot.GetOutline()->GetControlBox() + pen;
    pen += slot.GetAdvance();
  }

  return box;
}



} } } //namespace avs::text::freetype
