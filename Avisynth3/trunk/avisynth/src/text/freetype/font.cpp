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
#include "font.h"
#include "forward.h"
#include "face.h"
#include "glyphslot.h"
#include "outline.h"
#include "monobitmap.h"
#include "../../core/ownedblock.h"


namespace avs { namespace text { namespace freetype {


Font::Font(std::string const& name, int size)
{
  font_ = Face::Create (name);
  font_->SetCharSize (VecteurFP6 (0, size << 6),
		      DimensionFP6 (300, 300));
}

// Font API

DimensionFP6 Font::GetTextBoundingBox(std::string const& text)
{
  if ( text.size() == 0 )   //if no text
    return DimensionFP6();  //empty box

  int glyphCount = text.length();
  bool useKerning = font_->HasKerning();
  unsigned prevIndex = font_->GetCharIndex(text[0]);

  GlyphSlot slot (font_);
  slot.LoadGlyph (prevIndex);
  VecteurFP6 pen = slot.GetAdvance ();

  POutline outline = slot.GetOutline();
  BoxFP6 box = outline->GetControlBox();

  for( int n = 1; n < glyphCount; ++n )
  {
    unsigned glyphIndex = font_->GetCharIndex(text[n]);

    if ( useKerning )
      pen += font_->GetKerning(prevIndex, glyphIndex);

    slot.LoadGlyph (glyphIndex);
    outline = slot.GetOutline();

    box |= outline->GetControlBox() + pen;
    pen += slot.GetAdvance();
    prevIndex = glyphIndex;
  }

  return box.GetDimension();
}

// void Font::GetPositions (std::string const& text, VecteurFP6 *pos)
// {
//   bool useKerning = font_->HasKerning();
//   unsigned prevIndex = 0;

//   GlyphSlot slot (font_);
//   VecteurFP6 pen(0,0);

//   for ( unsigned int n = 0 ; n < text.length() ; n++)
//     {
//       unsigned glyphIndex = font_->GetCharIndex(text[0]);

//       if ( useKerning  && prevIndex && glyphIndex)
// 	pen += font_->GetKerning(prevIndex, glyphIndex);

//       pos[n] = pen;
//       slot.LoadGlyph (glyphIndex);
//       pen += slot.GetAdvance ();
//       prevIndex = glyphIndex;
//     }
// }

void Font::RenderingText (std::string const& text,
			  BufferWindow& result)
{
  if ( text.size() == 0 )   //if no text
    return;

  bool useKerning = font_->HasKerning();
  unsigned int prevIndex = 0;

  GlyphSlot slot (font_);
  VecteurFP6 pen(0,0);  // one could add coords in the method and set pen to them
  MonoBitmap bitmap(result);
  POutline outline;
  
  // set the buffer to 0.
  // if several call of RenderingText occurs on the same bw,
  // only the last one is taken into account.
  // Maybe one doesn't want that.
  memset (result.write(), 0, result.size());

  for (unsigned int n = 0 ; n < text.length() ; n++)
    {
      unsigned int glyphIndex = font_->GetCharIndex(text[n]);

      if ( useKerning  && prevIndex && glyphIndex)
	pen += font_->GetKerning(prevIndex, glyphIndex);

      slot.LoadGlyph (glyphIndex);
      outline = slot.GetOutline();
      outline->Translate (pen);
      outline->Draw (bitmap);
      outline->Translate (-pen);
      pen += slot.GetAdvance ();
      prevIndex = glyphIndex;
    }
}


} } } //namespace avs::text::freetype
