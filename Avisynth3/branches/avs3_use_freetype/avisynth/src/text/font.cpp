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
#include "font.h"
#include "../core/exception/generic.h"

//freetype includes
#include "../freetype/glyph.h"
#include "../freetype/face.h"


namespace avs { namespace text {


Font::Font(std::string const& name, int size)
  : font_( name, size )
{
  font_.SetCharSize(Dimension(0, size << 6), Dimension(300, 300));
}


Dimension Font::GetTextBoundingBox(std::string const& text)
{
  if ( text.size() == 0 )   //if no text
    return Dimension();     //empty box

  int glyphCount = text.length();
  bool useKerning = font_.HasKerning();

  unsigned prevIndex = font_.GetCharIndex(text[0]);
  int x = font_.GetAdvance(prevIndex).x >> 6;

  for( int n = 1; n < glyphCount; ++n )
  {
    unsigned glyphIndex = font_.GetCharIndex(text[n]);

    if ( useKerning )
      x += font_.GetKerning(prevIndex, glyphIndex).x >> 6;

  
  freetype::Glyph *glyphs = new freetype::Glyph[num_glyphs];
  Vecteur *pos = new Vecteur [num_glyphs];

  for ( int n = 0 ; n < glyphCount; ++n )
  {
    Vecteur advance;

    unsigned glyphIndex
    
    if ( use_kerning && previous && glyphIndex )
	  {
	    Vecteur delta = font_.GetKerning(previous, glyphIndex);
  	  x += delta.x >> 6;
	  }
    
    pos[n].x = x;
    pos[n].y = y;
    font_.LoadGlyph (glyph_index);
    glyphs[n]=font_.GetGlyph (glyph_index);
    advance = font_.GetAdvance (glyph_index);
    x += advance.x >> 6;
    previous = glyph_index;
  }

  Vecteur  bbox_min (32000, 32000);
  Vecteur  bbox_max (-32000, -32000);

  for ( int n = 0; n < glyphCount; ++n )
  {
    Vecteur min, max; 
    glyphs[n].GetCbox (ft_glyph_bbox_pixels, &min, &max);
    min += pos[n];
    max += pos[n];

    if ( min.x < bbox_min.x )
	    bbox_min.x = min.x;
    if ( min.y < bbox_min.y )
	    bbox_min.y = min.y;
      
    if ( max.x > bbox_max.x )
	    bbox_max.x = max.x;
    if ( max.y > bbox_max.y )
	    bbox_max.y = max.y;
  }

  if (bbox_min.x > bbox_max.x)
    {
      bbox_min = Vecteur (0, 0);
      bbox_max = Vecteur (0, 0);
    }

  delete [] glyphs;
  delete [] pos;

  width  = bbox_max.x - bbox_min.x;
  height = bbox_max.y - bbox_min.y;

  if (height == 0)
    throw exception::Generic("GetTextBoundingBox failure");

  return Dimension(width, height);
}



} } //namespace avs::text
