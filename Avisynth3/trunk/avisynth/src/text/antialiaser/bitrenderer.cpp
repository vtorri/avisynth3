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


//avisynth include
#include "bitrenderer.h"
#include "../gethfont.h"
#include "../../core/vecteur.h"


namespace avs { namespace text { namespace aliaser {


BitRenderer::BitRenderer(Dimension const& dim, Font const& font)
  : font_( font )
  , dim_( dim )
{
  struct {
    BITMAPINFOHEADER bih;
    RGBQUAD clr[2];
  } b;

  b.bih.biSize            = sizeof(BITMAPINFOHEADER);
  b.bih.biWidth           = dim.GetWidth() * 8;
  b.bih.biHeight          = dim.GetHeight() * 8;
  b.bih.biBitCount        = 1;
  b.bih.biPlanes          = 1;
  b.bih.biCompression     = BI_RGB;
  b.bih.biXPelsPerMeter   = 0;
  b.bih.biYPelsPerMeter   = 0;
  b.bih.biClrUsed         = 2;
  b.bih.biClrImportant    = 2;
  b.clr[0].rgbBlue = b.clr[0].rgbGreen = b.clr[0].rgbRed = 0;
  b.clr[1].rgbBlue = b.clr[1].rgbGreen = b.clr[1].rgbRed = 255;

  hdc_ = CreateCompatibleDC(NULL);
  HBITMAP hbm = CreateDIBSection( hdc_, (BITMAPINFO *)&b, DIB_RGB_COLORS, &antialiasBits_, NULL, 0 );

  hbmDefault_ = (HBITMAP)SelectObject(hdc_, hbm);
  hfontDefault_ = (HFONT)SelectObject(hdc_, GetHFONT()(font_));

  SetMapMode(hdc_, MM_TEXT);
  SetTextColor(hdc_, 0xffffff);
  SetBkColor(hdc_, 0);
}


BitRenderer::~BitRenderer()
{
  DeleteObject(SelectObject(hdc_, hbmDefault_));
  DeleteObject(SelectObject(hdc_, hfontDefault_));
  DeleteDC(hdc_);
}


void BitRenderer::SetText(std::string const& text, Vecteur const& coords, Align align)
{
  SetTextAlign( hdc_, ToWinAlign(align) );

  TextOut(hdc_, coords.x * 8 + 16, coords.y * 8 + 16, text.c_str(), text.length());

  GdiFlush();
}


CWindowPtr BitRenderer::ReadBits() const
{
  return CWindowPtr( (BYTE const *)antialiasBits_, (dim_.GetWidth() + 3) & -4, dim_.GetWidth(), dim_.GetHeight() * 8);
}


int BitRenderer::ToWinAlign(Align align)
{
  switch( align )
  { 
  case BOTTOM_LEFT:   return TA_BOTTOM   | TA_LEFT;
  case BOTTOM_CENTER: return TA_BOTTOM   | TA_CENTER;
  case BOTTOM_RIGHT:  return TA_BOTTOM   | TA_RIGHT;
  case CENTER_LEFT:   return TA_BASELINE | TA_LEFT;
  case CENTER:        return TA_BASELINE | TA_CENTER;
  case CENTER_RIGHT:  return TA_BASELINE | TA_RIGHT;
  case TOP_LEFT:      return TA_TOP      | TA_LEFT;
  case TOP_CENTER:    return TA_TOP      | TA_CENTER;
  case TOP_RIGHT:     return TA_TOP      | TA_RIGHT;
  }
  
  return TA_BASELINE | TA_LEFT;
}



} } } //namespace avs::text::aliaser
