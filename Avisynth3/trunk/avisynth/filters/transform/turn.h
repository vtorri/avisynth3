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


#ifndef __AVS_FILTERS_TURN_H__
#define __AVS_FILTERS_TURN_H__


//avisynth includes
#include "../../videoinfo.h"
#include "../simplifiable.h"
#include "../cachingclip_onechild_pipeline.h"


#pragma warning ( push )           //push warning state
#pragma warning (disable : 4250)   //get rid of MI dominance decisions


namespace avs { namespace filters {


//class declarations
class Flip;
class Symetry;



/////////////////////////////////////////////////////////////////////////////////////////
//  Turn
//
//  factorisation superclass for Turn::Left and Turn::Right
//
class Turn : public CachingClip::OneChild::Pipeline
           , public Simplifiable<Turn>
           , public Refactorable<Turn>
           , public Refactorable<Flip>
           , public Refactorable<Symetry>
{

  VideoInfo vi_;


public:  //constructor

  Turn(PClip child);


public:  //clip general interface

  virtual VideoInfo const& GetVideoInfo() const { return vi_; }


private:  //Refactor methods

  virtual PClip Refactor(Flip const& parent) const;

  virtual PClip Refactor(Symetry const& parent) const { return MirrorTurn(GetChild()); }


public:  //Turn interface

  virtual PClip MirrorTurn(PClip child) const = 0;

  virtual PClip TurnLeft() const = 0;
  virtual PClip TurnRight() const = 0;


public:  //implementation inner subclasses

  class Left;
  class Right;

};//Turn



////////////////////////////////////////////////////////////////////////////////////////
//  Turn::Left
//
//  filter to turn a video to the left (aka counter-clockwise)
//
class Turn::Left : public Turn
{

public:  //constructor

  Left(PClip child)
    : Turn( child ) { }


public:  //child changing clone

  virtual PClip clone(PClip child) const { return new Left(child); }


protected:  //ProcessFrame method
  
  virtual CPVideoFrame ProcessFrame(CPVideoFrame source) const;


private:  //Refactorable<Turn>

  virtual PClip Refactor(Turn const& parent) const { return parent.TurnLeft(); }


public:  //Turn interface

  virtual PClip MirrorTurn(PClip child) const;

  virtual PClip TurnLeft() const;
  virtual PClip TurnRight() const { return GetChild(); }

};


//////////////////////////////////////////////////////////////////////////////////
//  Turn::Right
//
//  filter to turn a video to the right (aka clockwise)
//
class Turn::Right : public Turn
{

public:  //constructor

  Right(PClip child) : Turn( child ) { }


public:  //child changing clone

  virtual PClip clone(PClip child) const { return new Right(child); }


protected:  //ProcessFrame method
  
  virtual CPVideoFrame ProcessFrame(CPVideoFrame source) const;


private:  //Refactorable<Turn>

  virtual PClip Refactor(Turn const& parent) const { return parent.TurnRight(); }


public:  //Turn interface

  virtual PClip MirrorTurn(PClip child) const;

  virtual PClip TurnLeft() const { return GetChild(); }
  virtual PClip TurnRight() const; 


};//Turn::Right





}; }; //namespace avs::filters

#pragma warning ( pop )

#endif //__AVS_FILTERS_TURN_H__