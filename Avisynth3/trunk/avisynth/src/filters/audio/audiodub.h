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


#ifndef __AVS_FILTERS_AUDIODUB_H__
#define __AVS_FILTERS_AUDIODUB_H__

//avisynth includes
#include "../../clip/refactorable.h"
#include "../../clip/twochilds/concrete.h"


namespace avs { namespace filters {

//class declarations
class KillAudio;
class KillVideo;


////////////////////////////////////////////////////////////////////////////////
//  AudioDub
//
//  clip to mux the audio track of one clip with the video of another
//  video is taken from left child, audio from right child
//
class AudioDub : public clip::twochilds::Concrete
               , public clip::Refactorable<KillAudio>
               , public clip::Refactorable<KillVideo>
{  

public:  //constructor

  AudioDub(PClip const& video, PClip const& audio);


public:  //clip general interface

  virtual CPVideoInfo GetVideoInfo() const;

  virtual CPVideoFrame GetFrame(int n) const { return GetRightFrame(n); }

  virtual void GetAudio(void * buffer, long long start, int count) const { GetLeftAudio(buffer, start, count); }


public:  //Simplify method

  virtual PClip Simplify() const;


private:  //Refactor methods

  virtual PClip Refactor(KillAudio const& parent) const;

  virtual PClip Refactor(KillVideo const& parent) const;


public:  //read access

  PClip const& GetVideoChild() const { return GetLeftChild(); }
  PClip const& GetAudioChild() const { return GetRightChild(); }

};



} } //namespace avs { namespace filters

#endif //__AVS_FILTERS_AUDIODUB_H__
