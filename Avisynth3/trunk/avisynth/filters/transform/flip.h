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


#ifndef __AVS_FILTERS_FLIP_H__
#define __AVS_FILTERS_FLIP_H__


//avisynth include
#include "../cachingclip_onechild_pipeline.h"


#pragma warning ( push )           //push warning state
#pragma warning (disable : 4250)   //get rid of MI dominance decisions


namespace avs { namespace filters {


////////////////////////////////////////////////////////////////////////////////////
//  FlipVertical
//
//  filter to flip the video vertically
//
class FlipVertical : public CachingClip::OneChild::Pipeline
{

public:  //constructor

  FlipVertical(PClip child)
    : Pipeline( child ) { }


public:  //child changing clone

  virtual PClip clone(PClip child) const { return new FlipVertical(child); }


protected:  //ProcessFrame method

  virtual CPVideoFrame ProcessFrame(CPVideoFrame source) const;


protected:  //Simplify method

  virtual PClip Simplify(PClip self) const;

};


/////////////////////////////////////////////////////////////////////////////////////
//  FlipHorizontal
//
//  filter to flip a video horizontally
//
class FlipHorizontal : public CachingClip::OneChild::Pipeline
{

public:  //constructor

  FlipHorizontal(PClip child)
    : Pipeline( child ) { }


public:  //child changing clone

  virtual PClip clone(PClip child) const { return new FlipHorizontal(child); }


protected:  //ProcessFrame method

  virtual CPVideoFrame ProcessFrame(CPVideoFrame source) const;


protected:  //Simplify method

  virtual PClip Simplify(PClip self) const;

};



}; }; //namespace avs::filters

#pragma warning ( pop )

#endif //__AVS_FILTERS_FLIPVERTICAL_H__