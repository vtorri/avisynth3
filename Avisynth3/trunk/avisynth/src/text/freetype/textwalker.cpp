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
#include "face.h"
#include "textwalker.h"


namespace avs { namespace text { namespace freetype {



TextWalker::TextWalker(PFace const& face, VecteurFP6 const& pen_)
  : slot_( face )
  , hasKerning_( face->HasKerning() )
  , pen( pen_ ) { }


Outline const& TextWalker::Reset(unsigned charCode)
{
  slot_.LoadGlyph( glyphIndex_ = slot_.GetFace()->GetCharIndex(charCode) );

  return slot_.GetOutline();
}


Outline const& TextWalker::LoadChar(unsigned charCode)
{
  unsigned index = slot_.GetFace()->GetCharIndex(charCode);

  pen += slot_.GetAdvance();
  if ( hasKerning_ )
    pen += slot_.GetFace()->GetKerning(glyphIndex_, index);

  slot_.LoadGlyph( glyphIndex_ = index );

  return slot_.GetOutline();
}


bool LineTokenize(std::string::const_iterator& begin, std::string::const_iterator end)
{
  assert( begin != end );

  switch( *begin )
  {
  case '\r': ++begin;
  case '\n': if ( begin != end && *begin == '\n' ) //if is for the case we fall through from 'r' to handle \r\n as newline
               ++begin;
             return true;

  default:   while ( begin != end && *begin != '\r' && *begin != '\n' )
               ++begin;
             return false;
  };
}



} } } //namespace avs::text::freetype
