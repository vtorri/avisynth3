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


#ifndef __AVS_FILTERS_SYMETRY_H__
#define __AVS_FILTERS_SYMETRY_H__


//avisynth include
#include "../simplifiable.h"
#include "../cachingclip_onechild_pipeline.h"


#pragma warning ( push )           //push warning state
#pragma warning (disable : 4250)   //get rid of MI dominance decisions


namespace avs { namespace filters {


//class declarations
class Turn;
class Flip;


class Symetry : public CachingClip::OneChild::Pipeline
              , public Simplifiable<Symetry>
              , public Refactorable<Turn>
              , public Refactorable<Flip>
              , public Refactorable<Symetry>
{

public:  //constructor

  Symetry(PClip child)
    : Pipeline( child ) { }


public:  //child changing clone

  virtual PClip clone(PClip child) const { return new Symetry(child); }


protected:  //ProcessFrame method

  virtual CPVideoFrame ProcessFrame(CPVideoFrame source) const;


private:  //Refactor methods

  virtual PClip Refactor(Turn const& parent) const;

  virtual PClip Refactor(Flip const& parent) const;

  virtual PClip Refactor(Symetry const& parent) const { return GetChild(); }

};




} } //namespace avs::filters

#pragma warning ( pop )

#endif //__AVS_FILTERS_SYMETRY_H__