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


#ifndef __AVS_FILTERS_KILLAUDIO_FOLDED_H__
#define __AVS_FILTERS_KILLAUDIO_FOLDED_H__

//avisynth includes
#include "../killaudio.h"
#include "../../../clip/folded.h"


#pragma warning ( push )           //push warning state
#pragma warning (disable : 4250)   //get rid of MI dominance decisions


namespace avs { namespace filters { namespace killaudio {



//////////////////////////////////////////////////////////////////////////////
//  killaudio::Folded
//
//  KillAudio subclass who folds its instance
//
class Folded : public clip::Folded<WeakPClip>
             , public KillAudio
{

public:  //structors

  Folded(PClip const& child)
    : KillAudio( child ) { }

  //generated destructor is fine


public:  //Simplify method

  virtual PClip Simplify() const
  {
    PClip sp = SimplifiableType::Simplify();
    
    return sp != shared_from_this() ? sp : FoldedType::Simplify();
  }


private:  //GetKey method

  virtual WeakPClip GetKey() const { return GetChild(); }

};



} } } //namespace avs::filters::killaudio

#pragma warning ( pop )

#endif //__AVS_FILTERS_KILLAUDIO_FOLDED_H__