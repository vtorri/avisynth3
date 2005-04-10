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


#ifndef __AVS_TEXT_FREETYPE_TEXTWALKER_H__
#define __AVS_TEXT_FREETYPE_TEXTWALKER_H__

//avisynth include
#include "glyphslot.h"

//boost include
#include <boost/optional.hpp>


namespace avs { namespace text { namespace freetype {




class TextWalker
{

  VecteurFP6 pen_;
  GlyphSlot slot_;
  bool hasKerning_;
  boost::optional<unsigned> glyphIndex_;


public:  //structors

  TextWalker(PFace const& face, VecteurFP6 const& pen);

  //generated destructor is fine


public:  //interface

  //LoadChar updates the pen to be the origin of the loaded char considering previous char (if there is any)
  //ie GetPen must be made AFTER the call to LoadChar
  Outline const& LoadChar(unsigned charCode);


public:  //access

  VecteurFP6 const& GetPen() const { return pen_; }
  void SetPen(VecteurFP6 const& pen) { pen_ = pen; glyphIndex_.reset(); }


public:

  typedef std::string::const_iterator iterator;

  void operator()(iterator begin, iterator end, rasterizer::OutlineSplitter& splitter);


public:

  static bool LineTokenize(std::string::const_iterator& begin, std::string::const_iterator end);

};


} } } //namespace avs::text::freetype

#endif //__AVS_TEXT_FREETYPE_TEXTWALKER_H__
