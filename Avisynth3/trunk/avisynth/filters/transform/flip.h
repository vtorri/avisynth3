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


//avisynth includes
#include "../simplifiable.h"
#include "../cachingclip_onechild_pipeline.h"


#pragma warning ( push )           //push warning state
#pragma warning (disable : 4250)   //get rid of MI dominance decisions


namespace avs { namespace filters {


//class declaration
class Symetry;



class Flip : public CachingClip::OneChild::Pipeline
           , public Simplifiable<Flip>
           , public Refactorable<Flip>
           , public Refactorable<Symetry>
{

public:  //constructor

  Flip(PClip child)
    : Pipeline( child ) { }


private:  //Refactor<Symetry>

  virtual PClip Refactor(Symetry const& parent) const { return OtherFlip(GetChild()); }


public:  //Flip interface

  virtual PClip OtherFlip(PClip child) const = 0;

  virtual PClip FlipVertical() const = 0;
  virtual PClip FlipHorizontal() const = 0;


public:  //implementation inner subclasses

  class Vertical;
  class Horizontal;

};


////////////////////////////////////////////////////////////////////////////////////
//  Flip::Vertical
//
//  filter to flip the video vertically
//
class Flip::Vertical : public Flip
{

public:  //constructor

  Vertical(PClip child)
    : Flip( child ) { }


public:  //child changing clone

  virtual PClip clone(PClip child) const { return new Vertical(child); }


protected:  //ProcessFrame method

  virtual CPVideoFrame ProcessFrame(CPVideoFrame source) const;


private:  //Refactorable<Flip>

  virtual PClip Refactor(Flip const& parent) const { return parent.FlipVertical(); }


public:  //Flip interface

  virtual PClip OtherFlip(PClip child) const;

  virtual PClip FlipVertical() const { return GetChild(); }
  virtual PClip FlipHorizontal() const;

};


/////////////////////////////////////////////////////////////////////////////////////
//  Flip::Horizontal
//
//  filter to flip a video horizontally
//
class Flip::Horizontal : public Flip
{

public:  //constructor

  Horizontal(PClip child)
    : Flip( child ) { }


public:  //child changing clone

  virtual PClip clone(PClip child) const { return new Horizontal(child); }


protected:  //ProcessFrame method

  virtual CPVideoFrame ProcessFrame(CPVideoFrame source) const;


private:  //Refactorable<Flip>

  virtual PClip Refactor(Flip const& parent) const { return parent.FlipHorizontal(); }


public:  //Flip interface

  virtual PClip OtherFlip(PClip child) const;

  virtual PClip FlipVertical() const; 
  virtual PClip FlipHorizontal() const { return GetChild(); }


};



}; }; //namespace avs::filters

#pragma warning ( pop )

#endif //__AVS_FILTERS_FLIPVERTICAL_H__