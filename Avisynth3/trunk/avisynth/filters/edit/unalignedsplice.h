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


#ifndef __AVS_UNALIGNEDSPLICE_H__
#define __AVS_UNALIGNEDSPLICE_H__

//avisynth include
#include "abstractsplice.h"


/////////////////////////////////////////////////////////////////////////////////
//  UnalignedSplice
//
//  Joins  clips otgether without trying to keep AV synch
//
class UnalignedSplice : public AbstractSplice
{

public:  //constructor

  UnalignedSplice(PClip child)
    : AbstractSplice( child->GetVideoInfo(), child->GetEnvironment() )
  {
    AddChild(child);
  }

  UnalignedSplice(PClip first, PClip second)
    : AbstractSplice( first->GetVideoInfo(), first->GetEnvironment() )
  {
    AddChild(first);
    AddChild(second);
  }


private:

  virtual const AbstractSplice * IsAbsorbable(const Clip * child) const
  {
    return dynamic_cast<const UnalignedSplice>(child);
  }

  virtual __int64 SamplesToSpliceFrom(const VideoInfo& vi) const
  {
    return vi.GetSamplesCount();
  }

};

#endif //__AVS_UNALIGNEDSPLICE_H__