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


#ifndef __AVS_FREETYPE_GLYPH_H__
#define __AVS_FREETYPE_GLYPH_H__

//avisynth includes
#include "../core/geometry/box.h"
#include "../core/geometry/vecteur.h"

//boost include
#include <boost/shared_ptr.hpp>


//declaration
struct FT_GlyphRec_;


namespace avs { namespace freetype {
  


class Glyph
{

  boost::shared_ptr<FT_GlyphRec_> glyph_;   //underlying handle
    

public:  //structors

  explicit Glyph(FT_GlyphRec_ * glyph);

  //generated copy constructor and destructor are fine


public:  //assignment

  //generated operator= is fine

  void swap(Glyph& other) { glyph_.swap(other.glyph_); }


public:  //API

  Vecteur GetAdvance() const;
  Box GetControlBox() const;

/*    void render           (FT_GlyphSlot    slot,
			   FT_Render_Mode render_mode);
    
    
    void transform        (FT_Matrix *matrix,
			   FT_Vector *delta);


    void convert_to_bitmap (FT_Render_Mode render_mode,
			    FT_Vector*     origin,
			    FT_Bool        destroy );*/

};


} } // namespace avs::freetype



#endif //__AVS_FREETYPE_GLYPH_H__
