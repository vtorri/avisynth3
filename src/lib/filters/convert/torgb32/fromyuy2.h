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


#ifndef __AVS_FILTERS_CONVERT_TORGB32_FROMYUY2_H__
#define __AVS_FILTERS_CONVERT_TORGB32_FROMYUY2_H__

//avisynth include
#include "../torgb32.h"


namespace avs { namespace filters { namespace convert { namespace torgb32 {



////////////////////////////////////////////////////////////////////////////////////////////////
//  convert::torgb32::FromYUY2
//
//  YUY2 to RGB32 code path
//
class FromYUY2 : public ToRGB32
{

public: //structors

  FromYUY2(PClip const& child)
    : ToRGB32( child ) { }

  //generated destructor is fine


private:  //Convert method

  virtual void ConvertFrame(VideoFrame const& source, VideoFrame& target) const;


public:  //free function version

  static void ConvertFrame(CWindowPtr src, WindowPtr dst);


public:  //factory method

  static PClip Create(PClip const& child) { return PClip( static_cast<Clip *>(new FromYUY2(child)) ); }

};


} } } } //namespace avs::filters::convert::torgb32

#endif //__AVS_FILTERS_CONVERT_TORGB32_FROMYUY2_H__
