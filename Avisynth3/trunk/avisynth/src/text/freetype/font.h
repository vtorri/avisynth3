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

//avisynth includes
#include "forward.h"                            //for TextWalker, PFace
#include "../../core/geometry/box.h"
#include "../../core/geometry/fixed_point.h"

//boost include
#include <boost/shared_ptr.hpp>


namespace avs { namespace text { namespace freetype {



////////////////////////////////////////////////////////////////////////////////////////
//  Font
//
//
//
class Font
{

  PFace face_;


public:

  Font(PFace const& face)
    : face_ ( face ) { }
  Font(std::string const& name, int size);

  //generated copy constructor and destructor are fine


public:  //Font interface
  
  BoxFP6 GetTextBoundingBox(std::string const& text) const;
  BoxFP6 GetTextLineBoundingBox(std::string::const_iterator begin, std::string::const_iterator end, TextWalker& walker) const;


public:  //access

  PFace const& GetFace() const { return face_; }

};



} } } //namespace avs::text::freetype

#endif //__AVS_TEXT_FREETYPE_FONT_H__
