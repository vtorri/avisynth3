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


#ifndef __AVS_FILTER_ASSUMEFPS_H__
#define __AVS_FILTER_ASSUMEFPS_H__

//avisynth includes
#include "../../clip/onechild/concrete.h"
#include "../../clip/onechild/videoprocess.h"


namespace avs { namespace filters {



//////////////////////////////////////////////////////////////////////////////////////////
//  AssumeFPS
//
//  filter to change the framerate without changing the frame count
//
class AssumeFPS : public clip::onechild::VideoProcess
                , public clip::onechild::Concrete
{

  CPVideoInfo vi_;


public:  //structors

  AssumeFPS(PClip const& child, Fraction const& fps);

  //generated destructor is fine


public:  //clip general interface

  virtual PEnvironment const& GetEnvironment() const { return GetChildEnvironment(); }
  virtual CPVideoInfo GetVideoInfo() const { return vi_; }

  virtual CPVideoFrame GetFrame(int n) const { return GetChildFrame(n); }
  virtual void GetAudio(void * buffer, __int64 start, int count) const { return GetChildAudio(buffer, start, count); }


public:  //child changing clone

  virtual PClip clone(PClip const& child) const;


public:  //factory method

  static PClip Create(PClip const& child, Fraction const& fps) { return PClip( (Clip *)new AssumeFPS(child, fps) ); }

};



} } //namespace avs::filters

#endif //__AVS_FILTER_ASSUMEFPS_H__
