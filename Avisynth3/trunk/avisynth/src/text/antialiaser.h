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


#ifndef __AVS_TEXT_ANTIALASER_H__
#define __AVS_TEXT_ANTIALASER_H__

//avisynth includes
#include "../core/bufferwindow.h"
#include "antialiaser/bitrenderer.h"

//boost include
#include <boost/utility.hpp>


namespace avs { namespace text {



//////////////////////////////////////////////////////////////////////////////////////
//  Antialiaser
//
//  helper class to anti-alias text
//
class Antialiaser : private boost::noncopyable
{  

  bool dirty_;
  BufferWindow alphaBuffer_;
  aliaser::BitRenderer bitRenderer_;


public:  //structors

  Antialiaser(Dimension const& dim, PEnvironment const& env, Font const& font);
 
  //generated destructor is fine
  

public:

  Dimension GetDimension() const { return alphaBuffer_.GetDimension().Divide<2, 1>(); }

  void SetText(std::string const& text, Vecteur const& coords, Align align)
  { 
    dirty_ = true; 
    bitRenderer_.SetText(text, coords, align); 
  }
  
  void Apply(VideoFrame& frame, int textcolor, int halocolor);
  

private:

  void ApplyRGB24(VideoFrame& frame, int textcolor, int halocolor);
  void ApplyRGB32(VideoFrame& frame, int textcolor, int halocolor);
  void ApplyYUY2(VideoFrame& frame, int textcolor, int halocolor);
  void ApplyYV12(VideoFrame& frame, int textcolor, int halocolor);
 
  void UpdateAlpha();

};



} } //namespace avs::text

#endif //__AVS_TEXT_ANTIALASER_H__
