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
#include "monobitmap.h"
#include "textwalker.h"
#include "bitrenderer.h"
#include "../../core/ownedblock.h"

//assert include
#include <assert.h>


namespace avs { namespace text { namespace freetype {



BufferWindow BitRenderer::operator()(std::string const& text, VecteurFP6 const& pen, Align align) const
{
  BufferWindow bw(dim_, env_);
  // set the buffer to 0.
  // if several call of RenderingText occurs on the same bw,
  // only the last one is taken into account.
  // Maybe one doesn't want that.
  memset (bw.write(), 0, bw.size());

  if ( text.size() == 0 )   //if no text, return a black bw
    return bw;

  Draw( text.begin(), text.end(), TextWalker(face_, GetFTOrigin(pen, Font(face_).GetTextBoundingBox(text), align)), MonoBitmap(bw) );

  return bw;
}


VecteurFP6 BitRenderer::GetFTOrigin(VecteurFP6 pen, BoxFP6 const& box, Align align)
{
  pen -= box.GetPosition();

  switch( align )
  {
  //case BOTTOM_LEFT:   return pen;
  case BOTTOM_CENTER: return pen + VecteurFP6( -box.GetDimension().GetWidth() >> 1, 0 );
  case BOTTOM_RIGHT:  return pen + VecteurFP6( -box.GetDimension().GetWidth(), 0 );
  case CENTER_LEFT:   return pen + VecteurFP6( 0, -box.GetDimension().GetHeight() >> 1 );
  case CENTER:        return pen + VecteurFP6( -box.GetDimension().GetWidth() >> 1, -box.GetDimension().GetHeight() >> 1 );
  case CENTER_RIGHT:  return pen + VecteurFP6( -box.GetDimension().GetWidth(), -box.GetDimension().GetHeight() >> 1 );
  case TOP_LEFT:      return pen + VecteurFP6( 0, -box.GetDimension().GetHeight() );
  case TOP_CENTER:    return pen + VecteurFP6( -box.GetDimension().GetWidth(), -box.GetDimension().GetHeight() );
  case TOP_RIGHT:     return pen + VecteurFP6( -box.GetDimension().GetWidth(), -box.GetDimension().GetHeight() );
  }

  assert( align == BOTTOM_LEFT );
  return pen;
}
					  

void BitRenderer::Draw(std::string::const_iterator begin, std::string::const_iterator end, TextWalker& walker, MonoBitmap const& bitmap) const
{
  assert( begin != end );    //method shouldn't be called for naught

  Outline outline = walker.Reset(*begin++);
    
  outline.Translate(walker.pen);    
  outline.Draw(bitmap);

  while ( begin != end )
  {
    outline = walker.LoadChar(*begin++);

    outline.Translate(walker.pen);
    outline.Draw(bitmap);
  }  
}



} } } //namespace avs::text::freetype
