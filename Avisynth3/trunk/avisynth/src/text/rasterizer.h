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


#ifndef __AVS_TEXT_RASTERIZER_H__
#define __AVS_TEXT_RASTERIZER_H__

//avisynth includes
#include "align.h"
#include "freetype/font.h"
#include "freetype/forward.h"     //for VecteurFP6, TextWalker
#include "rasterizer/forward.h"   //for SpanMaker

//stl include
#include <string>


namespace avs { namespace text {



class Rasterizer
{

  freetype::Font font_;
  float radius_;


public:  //structors

  Rasterizer(freetype::Font const& font, float radius = 0.0f)
    : font_( font )
    , radius_( radius ) { }

  //generated copy constructor and destructor are fine


public:  

  void SetFont(freetype::Font const& font) { font_ = font; }
  void SetRadius(float radius) { radius_ = radius; }

  void operator()(std::string const& text, WindowPtr const& masks, freetype::VecteurFP6 const& pen, Align align) const;


public:

  static void ApplyTo(CWindowPtr const& masks, int textColor, int haloColor, VideoFrame& frame);


private:

  static void ApplyRGB24(CWindowPtr masks, int textColor, int haloColor, VideoFrame& frame);
  static void ApplyRGB32(CWindowPtr masks, int textColor, int haloColor, VideoFrame& frame);
  static void ApplyYUY2(CWindowPtr masks, int textColor, int haloColor, VideoFrame& frame);
  static void ApplyYV12(CWindowPtr masks, int textColor, int haloColor, VideoFrame& frame);

};



} } //namespace avs::text

#endif //__AVS_TEXT_RASTERIZER_H__
