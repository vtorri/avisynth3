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


#ifndef __AVS_DISSOLVE_H__
#define __AVS_DISSOLVE_H__


//avisynth include
#include "../generictwochildsclip.h"


#pragma warning ( push )           //push warning state
#pragma warning (disable : 4250)   //get rid of MI dominance decisions



//////////////////////////////////////////////////////////////////////////////////
//  Dissolve
//
//  filter to smoothly transition from one clip to another
//  the two clips must be compatible and have the same number of video frames
//  audio length is adjusted to keep synch
//
class Dissolve : public GenericTwoChildsClip 
{

public:  //constructor

  Dissolve(PClip left, PClip right)
    : GenericTwoChildsClip( left, right )
  {
    const VideoInfo& left_vi = left->GetVideoInfo();
    const VideoInfo& right_vi = right->GetVideoInfo();

    left_vi.CheckCompatible(right_vi);     //throw the appropriate exceptions
    left_vi.CheckLenghtMatch(right_vi);
    
    SetVideoInfo(left_vi.SetAudioToVideoSynch())
  }


public:  //clip general interface

  virtual void GetAudio(BYTE * buf, __int64 start, __int64 count) const;


protected:  //CachingClip MakeFrame method

  virtual CPVideoFrame MakeFrame(int n) const;

};


#pragma warning ( pop )

#endif //__AVS_DISSOLVE_H__