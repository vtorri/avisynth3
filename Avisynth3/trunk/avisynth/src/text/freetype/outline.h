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


#ifndef __AVS_TEXT_FREETYPE_OUTLINE_H__
#define __AVS_TEXT_FREETYPE_OUTLINE_H__

//avisynth include
#include "forward.h"
#include "../../core/geometry/box.h"

//freetype includes
#include <ft2build.h>
#include FT_OUTLINE_H


namespace avs { namespace text { namespace freetype {



////////////////////////////////////////////////////////////////////////////////////////
//  Outline
//
//  wrapper class around the native freetype FT_Outline
//
class Outline : public FT_Outline
{

private:  //declared but not implemented

  Outline();
  ~Outline();
  Outline(Outline const& other);

  Outline& operator=(Outline const& other);


public:  //clone method

  POutline clone() const;


public:  //Outline interface

  void Translate(long x, long y);

  Box GetControlBox() const;

  void Draw(MonoBitmap const& bitmap) const;

};



} } } //namespace avs::text::freetype

#endif //__AVS_TEXT_FREETYPE_OUTLINE_H__
