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


#ifndef __AVS_FILTERS_TURN_H__
#define __AVS_FILTERS_TURN_H__

//avisynth includes
#include "../../clip/caching/concrete.h"
#include "../../clip/onechild/concrete.h"
#include "../../clip/onechild/cachingpipeline.h"


namespace avs { namespace filters {


//class declarations
class Flip;
class Symetry;



/////////////////////////////////////////////////////////////////////////////////////////
//  Turn
//
//  factorisation superclass for Turn::Left and Turn::Right
//
class AVS_NOVTABLE Turn : public clip::onechild::CachingPipeline
                        , public clip::onechild::Simplifiable<Turn>
                        , public clip::Refactorable<Turn>
                        , public clip::Refactorable<Flip>
                        , public clip::Refactorable<Symetry>
                        , public clip::onechild::Concrete
                        , public clip::caching::Concrete
{

public:  //constructor

  Turn(PClip const& child)
    : clip::onechild::Concrete( child )
    , clip::caching::Concrete( child->GetEnvironment() ) { }


public:  //clip general interface

  virtual CPVideoInfo GetVideoInfo() const;


private:  //Refactor methods

  virtual PClip Refactor(Flip const& parent) const;

  virtual PClip Refactor(Symetry const& /*parent*/) const { return MirrorTurn(GetChild()); }


public:  //Turn interface

  virtual PClip MirrorTurn(PClip const& child) const = 0;

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

  Left(PClip const& child)
    : Turn( child ) { }


public:  //child changing clone

  virtual PClip clone(PClip const& child) const { return Create(child); }


protected:  //Pipeline interface

  virtual CPVideoFrame MakeFrame(CPVideoFrame const& source) const;


private:  //Refactorable<Turn>

  virtual PClip Refactor(Turn const& parent) const { return parent.TurnLeft(); }


public:  //Turn interface

  virtual PClip MirrorTurn(PClip const& child) const;

  virtual PClip TurnLeft() const;
  virtual PClip TurnRight() const { return GetChild(); }


public:  //factory method

  static PClip Create(PClip const& child) { return PClip( static_cast<Clip *>(new Left(child)) )->Simplify(); }

};


//////////////////////////////////////////////////////////////////////////////////
//  Turn::Right
//
//  filter to turn a video to the right (aka clockwise)
//
class Turn::Right : public Turn
{

public:  //constructor

  Right(PClip const& child)
    : Turn( child ) { }


public:  //child changing clone

  virtual PClip clone(PClip const& child) const { return Create(child); }


protected:  //Pipeline interface

  virtual CPVideoFrame MakeFrame(CPVideoFrame const& source) const;


private:  //Refactorable<Turn>

  virtual PClip Refactor(Turn const& parent) const { return parent.TurnRight(); }


public:  //Turn interface

  virtual PClip MirrorTurn(PClip const& child) const;

  virtual PClip TurnLeft() const { return GetChild(); }
  virtual PClip TurnRight() const; 


public:  //factory method

  static PClip Create(PClip const& child) { return PClip( static_cast<Clip *>(new Right(child)) )->Simplify(); }

};//Turn::Right





} } //namespace avs::filters

#endif //__AVS_FILTERS_TURN_H__
