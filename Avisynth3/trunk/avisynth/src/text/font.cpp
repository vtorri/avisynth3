// Avisynth v3.0 alpha.  Copyright 2004 Ben Rudiak-Gould et al.
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
#include "../core/exception.h"

//boost include
#include <boost/bind.hpp>


namespace avs { namespace text {



Font::Font(std::string const& name, int size, bool bold, bool italic)
{
  //try new before creating so thazt we don't miss a DeleOBject through bad_alloc
  HFONT * pFont = new HFONT(  CreateFont( size, 0, 0, 0, bold ? FW_BOLD : FW_NORMAL, 
                     italic, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                     CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE | DEFAULT_PITCH, name.c_str() )  );
  if ( *pFont == NULL )
    throw Exception("Unable to create font");

  pFont_.reset( pFont, HFONTDeleter() );
}



Dimension Font::GetTextBoundingBox(std::string const& text)
{
  HDC hdc = GetDC(NULL);

  HFONT oldHFont = (HFONT)SelectObject(hdc, *pFont_);
  int oldMapMode = SetMapMode(hdc, MM_TEXT);
  //UINT oldAlign = SetTextAlign(hdc, align);

  RECT r = { 0, 0, 0, 0 };

  int height = DrawText(hdc, text.c_str(), text.length(), &r, DT_CALCRECT | DT_NOPREFIX);

  //SetTextAlign(hdc, old_text_align);
  SetMapMode(hdc, oldMapMode);
  SelectObject(hdc, oldHFont);

  ReleaseDC(NULL, hdc);

  if ( height == 0 )
    throw Exception("GetTextBoundingBox failure");

  return Dimension(r.right + 8, height + 8);
}



} } //namespace avs::text