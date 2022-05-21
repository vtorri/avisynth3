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


#ifndef __AVS_FILTERS_STATICIMAGE_H__
#define __AVS_FILTERS_STATICIMAGE_H__

//avisynth include
#include "../../clip/nochild.h"

//stl include
#include <string>


namespace avs { namespace filters {



////////////////////////////////////////////////////////////////////////////////
//  StaticImage
//
//
//
class StaticImage : public clip::NoChild
{

  CPVideoInfo vi_;
  CPVideoFrame frame_;


protected:  //structors

  StaticImage(CPVideoFrame const& frame);

  StaticImage(CPVideoFrame const& frame, CPVideoInfo const& vi);

  //generated destructor is fine


public:  //clip general interface

  virtual PEnvironment const& GetEnvironment() const;
  virtual CPVideoInfo GetVideoInfo() const { return vi_; }

  virtual CPVideoFrame GetFrame(long /*n*/) const { return frame_; }
  virtual BYTE * GetAudio(BYTE * buffer, long long start, long count) const;


public:  //factory methods

  static PClip Create(CPVideoFrame const& frame) { return PClip( static_cast<Clip *>(new StaticImage(frame)) ); }

  static PClip CreateBlankClip(PColorSpace const& space, Dimension const& dim, PEnvironment const& env);

};



} } //namespace avs::filters

#endif //__AVS_FILTERS_STATICIMAGE_H__
