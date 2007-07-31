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


#ifndef __AVS_TEXT_FREETYPE_GLYPHSLOT_H__
#define __AVS_TEXT_FREETYPE_GLYPHSLOT_H__

//avisynth include
#include "forward.h"

//boost include
#include <boost/shared_ptr.hpp>     //so PFace is defined
#include <boost/noncopyable.hpp>


//freetype declarations
struct FT_GlyphSlotRec_;
typedef FT_GlyphSlotRec_ * FT_GlyphSlot;


namespace avs { namespace text { namespace freetype {



/////////////////////////////////////////////////////////////////////////////
//  freetype::GlyphSlot
//
//  
//
class GlyphSlot : public boost::noncopyable
{

  PFace face_;
  FT_GlyphSlot glyphslot_;    //shortcut to the slot inside face_


public:  //structors
    
  GlyphSlot(PFace const& face);
  
  //generated destructor is fine


public:  //GlyphSlot interface

  //loads a glyph in the slot
  void LoadGlyph(unsigned glyphIndex);

  //fetchs glyph advance
  VecteurFP6 GetAdvance() const;

  //fetchs glyph outline
  Outline const& GetOutline() const;


public:  //access

  PFace const& GetFace() const { return face_; }

};



} } } // namespace avs::text::freetype

#endif //__AVS_TEXT_FREETYPE_GLYPHSLOT_H__
