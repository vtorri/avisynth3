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


#ifndef __AVS_CLIP_MANYCHILDS_H__
#define __AVS_CLIP_MANYCHILDS_H__

//avisynth include
#include "../core/clip.h"


namespace avs { namespace clip {



//////////////////////////////////////////////////////////////////////////////////////
//  ManyChilds
//
//  clip subinterface for clip with an unlimited number of childs
//
class NOVTABLE ManyChilds : public virtual Clip
{

public:  //access to childs

  virtual int GetChildCount() const = 0;

  //undefined behavior if n < 0 or >= GetChildCount()
  virtual PClip const& GetChild(int n) const = 0;

};



} } //namespace avs::clip

#endif  //__AVS_CLIP_MANYCHILDS_H__
