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


#ifndef __AVS_TEXT_FREETYPE_BITRENDERER_H__
#define __AVS_TEXT_FREETYPE_BITRENDERER_H__

//avisynth includes
#include "font.h"
#include "forward.h"
#include "../../core/bufferwindow.h"
#include "../../core/geometry/dimension.h"


namespace avs { namespace text { namespace freetype {



class BitRenderer
{

  PFace face_;
  Dimension dim_;
  PEnvironment env_;


public:  //structors

  BitRenderer(Font const& font, Dimension const& dim, PEnvironment const& env)
    : face_( font.GetFace() )
    , dim_( dim )
    , env_( env ) { }
 
  //generated copy constructor and destructor are fine


public: 

  BufferWindow operator()(std::string const& text, VecteurFP6 const& pen, Align align) const;


private:

  static VecteurFP6 GetFreeTypeOrigin(VecteurFP6 pen, BoxFP6 const& box, Align align);

  void Draw(char * begin, char * end, VecteurFP6 pen, MonoBitmap const& target) const;

};


} } } //namespace avs::text::freetype

#endif // __AVS_TEXT_FREETYPE_BITRENDERER_H__
