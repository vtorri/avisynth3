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


#ifndef __AVS_FILTERS_FLIP_H__
#define __AVS_FILTERS_FLIP_H__

//avisynth includes
#include "../../clip/caching/concrete.h"
#include "../../clip/onechild/concrete.h"
#include "../../clip/onechild/cachingpipeline.h"


namespace avs { namespace filters {


//declarations
class Symetry;



///////////////////////////////////////////////////////////////////////////////////////
//  Flip
//
//  factorisation superclass for Flip::Vertical and Flip::Horizontal
//
class AVS_NOVTABLE Flip : public clip::onechild::CachingPipeline
                        , public clip::onechild::Simplifiable<Flip>
                        , public clip::Refactorable<Flip>
                        , public clip::Refactorable<Symetry>
                        , public clip::onechild::Concrete
                        , public clip::caching::Concrete
{

public:  //constructor

  Flip(PClip const& child)
    : clip::onechild::Concrete( child )
    , clip::caching::Concrete( child->GetEnvironment() ) { }


public:  //clip general interface

  virtual CPVideoInfo GetVideoInfo() const { return GetChildVideoInfo(); }


private:  //Refactor<Symetry>

  virtual PClip Refactor(Symetry const& /*parent*/) const { return OtherFlip(GetChild()); }


public:  //Flip interface

  virtual PClip OtherFlip(PClip const& child) const = 0;

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

  Vertical(PClip const& child)
    : Flip( child ) { }


public:  //child changing clone

  virtual PClip clone(PClip const& child) const { return Create( child ); }


protected:  //Pipeline interface

  virtual CPVideoFrame MakeFrame(CPVideoFrame const& source) const;


private:  //Refactorable<Flip>

  virtual PClip Refactor(Flip const& parent) const { return parent.FlipVertical(); }


public:  //Flip interface

  virtual PClip OtherFlip(PClip const& child) const;

  virtual PClip FlipVertical() const { return GetChild(); }
  virtual PClip FlipHorizontal() const;


public:  //factory method

  static PClip Create(PClip const& child) { return PClip( (Clip *)new Vertical(child) ); }

};


/////////////////////////////////////////////////////////////////////////////////////
//  Flip::Horizontal
//
//  filter to flip a video horizontally
//
class Flip::Horizontal : public Flip
{

public:  //constructor

  Horizontal(PClip const& child)
    : Flip( child ) { }


public:  //child changing clone

  virtual PClip clone(PClip const& child) const { return Create( child ); }


protected:  //Pipeline interface

  virtual CPVideoFrame MakeFrame(CPVideoFrame const& source) const;


private:  //Refactorable<Flip>

  virtual PClip Refactor(Flip const& parent) const { return parent.FlipHorizontal(); }


public:  //Flip interface

  virtual PClip OtherFlip(PClip const& child) const;

  virtual PClip FlipVertical() const; 
  virtual PClip FlipHorizontal() const { return GetChild(); }


public:  //factory method

  static PClip Create(PClip const& child) { return PClip( (Clip *)new Horizontal(child) ); }

};



} } //namespace avs::filters

#endif //__AVS_FILTERS_FLIPVERTICAL_H__
