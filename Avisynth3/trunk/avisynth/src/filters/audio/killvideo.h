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


#ifndef __AVS_FILTERS_KILLVIDEO_H__
#define __AVS_FILTERS_KILLVIDEO_H__

//avisynth includes
#include "../../clip/onechild/concrete.h"
#include "../../clip/onechild/simplifiable.h"


namespace avs { namespace filters {



////////////////////////////////////////////////////////////////////////////////////
//  KillVideo
//
//  Removes the video from its child
//
class KillVideo : public clip::onechild::Simplifiable<KillVideo>
                , public clip::onechild::Concrete
{

protected:  //structors

  KillVideo(PClip const& child)
    : Concrete( child ) { }

  //generated destructor is fine


public:  //clip general interface

  virtual PEnvironment const& GetEnvironment() const { return GetChildEnvironment(); }
  virtual CPVideoInfo GetVideoInfo() const;

  virtual CPVideoFrame GetFrame(int n) const;
  virtual BYTE * GetAudio(BYTE * buffer, long long start, int count) const { return GetChildAudio(buffer, start, count); }


public:  //Simplify method

  virtual PClip Simplify() const;


public:  //child changing clone

  virtual PClip clone(PClip const& child) const { return Create(child); }


protected:  //GetKey method

  WeakPClip GetKey() const { return GetChild(); }


public:  //factory method and functor

  static PClip Create(PClip const& child); 

  struct Creator
  {
    PClip operator()(PClip const& child) const { return Create(child); }
  };

};



} } //namespace avs::filters

#endif //__AVS_FILTERS_KILLVIDEO_H__
