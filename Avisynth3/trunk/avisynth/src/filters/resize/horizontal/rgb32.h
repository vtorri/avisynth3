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


#ifndef __AVS_FILTERS_RESIZE_HORIZONTAL_RGB32_H__
#define __AVS_FILTERS_RESIZE_HORIZONTAL_RGB32_H__

//avisynth include
#include "rgb.h"


#pragma warning ( push )           //push warning state
#pragma warning (disable : 4250)   //get rid of MI dominance decisions


namespace avs { namespace filters { namespace resize { namespace horizontal {



//////////////////////////////////////////////////////////////////////////////////////
//  resize::horizontal::RGB32
//
//  resize horizontal RGB32 code path
//
class RGB32 : public RGB
{

public:  //structors

  RGB32(PClip const& child, PFilter const& filter, int width, SubRange const& subrange)
    : RGB( child, filter, width, subrange ) { }

  //generated destructor is fine


private:  //Resize method

  virtual void ResizeFrame(VideoFrame const& source, VideoFrame& target) const;

};



} } } } //namespace avs::filters::resize::horizontal

#pragma warning ( pop )

#endif //__AVS_FILTERS_RESIZE_HORIZONTAL_RGB32_H__