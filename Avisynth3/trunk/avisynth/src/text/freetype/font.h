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


#ifndef __AVS_TEXT_FREETYPE_FONT_H__
#define __AVS_TEXT_FREETYPE_FONT_H__

//avisynth include
#include "forward.h"
#include "../../core/geometry/box.h"
#include "../../core/geometry/fixed_point.h"
#include "../../core/bufferwindow.h"


namespace avs { namespace text { namespace freetype {



////////////////////////////////////////////////////////////////////////////////////////
//  Font
//
//  wrapper class around a PFace
//
class Font
{

  PFace font_;

public:

  Font(std::string const& name, int size);

  //generated copy constructor and destructor are fine


public:  // Font API
  
  DimensionFP6 GetTextBoundingBox(std::string const& text);

/*   void GetPositions (std::string const& text, VecteurFP6 *pos); */

  void RenderingText (std::string const& text,
		      BufferWindow&      result);
};



} } } //namespace avs::text::freetype

#endif //__AVS_TEXT_FREETYPE_OUTLINE_H__