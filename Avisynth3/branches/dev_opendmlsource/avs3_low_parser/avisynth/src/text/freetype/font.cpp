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
#include "textwalker.h"


namespace avs { namespace text { namespace freetype {



Font::Font(std::string const& name, int size)
  : face_( Face::Create(name) )
{
  face_->SetCharSize( VecteurFP6(0, size), Dimension(300, 300) );
}



BoxFP6 Font::GetTextBoundingBox(std::string const& text) const
{
  BoxFP6 result;              //init empty box

  if ( text.length() != 0 )   //only work if some text, else empty box
  {
    TextWalker walker(face_, VecteurFP6());

    result = GetTextLineBoundingBox( text.begin(), text.end(), walker );
  }

  return result;
}



BoxFP6 Font::GetTextLineBoundingBox(std::string::const_iterator begin, std::string::const_iterator end, TextWalker& walker) const
{
  BoxFP6 result;

  while ( begin != end )
  {
    Outline const& outline = walker.LoadChar(*begin++);  //NB: extra line coz LoadChar must be done before reading pen
    result |= outline.GetControlBox() + walker.GetPen();
  }

  return result;
}


} } } //namespace avs::text::freetype
