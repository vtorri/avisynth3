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


#ifndef __AVS_CLIP_ONECHILD_VIDEOPROCESS_H__
#define __AVS_CLIP_ONECHILD_VIDEOPROCESS_H__

//avisynth includes
#include "../onechild.h"
#include "../../filters/audio/killaudio.h"
#include "../../filters/audio/killvideo.h"


namespace avs { namespace clip { namespace onechild {



////////////////////////////////////////////////////////////////////////////
//  VideoProcess
//
//  only works on video and just forwards audio from child
//  so provides appropriate Refactor behavior for KillAudio and KillVideo
//
class NOVTABLE VideoProcess : public virtual OneChild
                            , public Refactorable<filters::KillVideo>
                            , public FinalRefactorable<filters::KillAudio>
{

public:  //clip general interface

  virtual BYTE * GetAudio(BYTE * buffer, long long start, int count) const { return GetChild()->GetAudio(buffer, start, count); }


private:  //Refactor methods

  virtual PClip Refactor(filters::KillVideo const& parent) const
  {
    return parent.clone( GetChild() );
  }

  virtual PClip FinalRefactor(filters::KillAudio const& parent) const
  {
    return clone( parent.clone(GetChild()) )->FinalSimplify();
  }

};



} } } //namespace avs::clip::onechild

#endif //__AVS_CLIP_ONECHILD_VIDEOPROCESS_H__
