// Avisynth v3.0 alpha.  Copyright 2003 Ben Rudiak-Gould et al.
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


#ifndef __AVS_FILTERS_VOIDCLIP_H__
#define __AVS_FILTERS_VOIDCLIP_H__

//avisynth includes
#include "../../clip/folded.h"
#include "../../clip/nochild.h"


#pragma warning ( push )           //push warning state
#pragma warning (disable : 4250)   //get rid of MI dominance decisions


namespace avs { namespace filters {



////////////////////////////////////////////////////////////////////////////////////
//  VoidClip
//
//  clip who has no video and no audio
//
class VoidClip : public clip::NoChild
               , public clip::Folded<WeakPEnvironment>
{

  PEnvironment env_;


public:  //constructor

  VoidClip(PEnvironment const& env)
    : env_( env ) { }


public:  //Clip general interface

  virtual PEnvironment const& GetEnvironment() const { return env_; }
  virtual CPVideoInfo GetVideoInfo() const;

  virtual CPVideoFrame GetFrame(int n) const;
  virtual void GetAudio(void * buf, __int64 start, int count) const;


private:  //GetKey method

  virtual KeyType GetKey() const { return env_; }


public:  //factory method

  static PClip Create(PEnvironment const& env) { return PClip( (Clip *)new VoidClip(env) ); }

};


} } //namespace avs::filters

#pragma warning ( pop )

#endif //__AVS_FILTERS_VOIDCLIP_H__