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


#ifndef __AVS_FILTERS_CONVERT_TOYV12_FROMYUY2_H__
#define __AVS_FILTERS_CONVERT_TOYV12_FROMYUY2_H__

//avisynth include
#include "../toyv12.h"


namespace avs { namespace filters { namespace convert { namespace toyv12 {



////////////////////////////////////////////////////////////////////////////////////////////////
//  convert::toyv12::FromYUY2
//
//  YUYU2 to YV12 code path
//
class FromYUY2 : public ToYV12
{

public: //structors

  FromYUY2(PClip const& child)
    : ToYV12( child ) { }

  //generated destructor is fine


private:  //Convert method

  virtual void ConvertFrame(VideoFrame const& source, VideoFrame& target) const;
  virtual void ConvertFrame(CWindowPtr src, WindowPtr Y, WindowPtr U, WindowPtr V) const;


public:  //factory method

  static PClip Create(PClip const& child) { return PClip( static_cast<Clip *>(new FromYUY2(child)) ); }

};


} } } } //namespace avs::filters::convert::toyv12

#endif //__AVS_FILTERS_CONVERT_TOYV12_FROMYUY2_H__
