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


#ifndef __AVS_FILTERS_KILLAUDIO_H__
#define __AVS_FILTERS_KILLAUDIO_H__

//avisynth includes
#include "../editfilter.h"
#include "../simplifiableandfolded.h"


#pragma warning ( push )           //push warning state
#pragma warning (disable : 4250)   //get rid of MI dominance decisions


namespace avs { namespace filters {


////////////////////////////////////////////////////////////////////////////////////
//  KillAudio
//
//  Removes the audio from its child
//
class KillAudio : public EditFilter
                , public SimplifiableAndFolded<KillAudio>
{

public:  //constructor

  KillAudio(PClip child)
    : EditFilter( child )
  {
    VideoInfo& vi = GetVideoInfo();
    
    vi.KillAudio();
  }


public:  //clip general interface

  virtual void GetAudio(BYTE * buf, __int64 start, __int64 count) const
  {
    ThrowNoAudioException();
  }


public:  //child changing clone

  virtual Clip * clone(PClip child) const { return new KillAudio(child); }


public:  //Simplify method

  virtual PClip Simplify(PClip self) const;


public:  //operator== needed by folding code

  bool operator==(const KillAudio& other) const { return GetChild() == other.GetChild(); }

};


}; }; //namespace avs::filters

#pragma warning ( pop )

#endif __AVS_FILTERS_KILLAUDIO_H__