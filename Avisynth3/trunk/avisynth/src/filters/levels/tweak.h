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


#ifndef __AVS_FILTERS_TWEAK_H__
#define __AVS_FILTERS_TWEAK_H__

//avisynth include
#include "../../clip/caching/concrete.h"
#include "../../clip/onechild/concrete.h"
#include "../../clip/onechild/cachingpipeline.h"


#pragma warning ( push )           //push warning state
#pragma warning (disable : 4250)   //get rid of MI dominance decisions


namespace avs { namespace filters {



/////////////////////////////////////////////////////////////////////////////
//  Tweak
//
//
class Tweak : public clip::onechild::CachingPipeline
            , public clip::onechild::Concrete  
            , public clip::caching::Concrete
{

protected:  //members

  int Sin, Cos;         // = (int) sin, cos  ( (hue * 3.1415926) / 180.0 )
  int Sat;              // = (int) sat * 512
  int Bright_p16;       // = (int) bright + 16        
  int Cont;             // = (int) cont * 512


public:  //constructors

  Tweak(PClip const& child, double hue, double sat, double bright, double cont);

  Tweak(PClip const& child, Tweak const& other);


public:  //Clip general interface

  virtual CPVideoInfo GetVideoInfo() const { return GetChild()->GetVideoInfo(); }

  virtual void GetAudio(void * buffer, __int64 start, int count) const { return GetChildAudio(buffer, start, count); }


public:  //child changing clone

  virtual PClip clone(PClip const& child) const;


public:  //factory method 

  static PClip Create(PClip child, double hue, double sat, double bright, double cont);


private:  //colorspace specific inner subclasses

  class YUY2;
  class YV12;

};

  
/////////////////////////////////////////////////////////////////////////////
//  Tweak::YUY2
//
//  Tweak subclass dealing with the YUY2 color space
//
class Tweak::YUY2 : public Tweak
{

public:  //constructors

  YUY2(PClip child, double hue, double sat, double bright, double cont)
    : Tweak(child, hue, sat, bright, cont) { }

  YUY2(PClip child, Tweak const& other)
    : Tweak(child, other) { }


protected:  //Pipeline MakeFrame method

  virtual CPVideoFrame MakeFrame(CPVideoFrame const& source) const;


public:  //special codepath inner subclasses

  class ISSE;

};

/////////////////////////////////////////////////////////////////////////////
//  Tweak::YUY2::ISSE
//
//  ISSE codepath for Tweak::YUY2
//
class Tweak::YUY2::ISSE : public Tweak::YUY2
{

public:  //constructors

  ISSE(PClip child, double hue, double sat, double bright, double cont)
    : YUY2(child, hue, sat, bright, cont) { }

  ISSE(PClip child, Tweak const& other)
    : YUY2(child, other) { }


protected:  //Pipeline MakeFrame method

  virtual CPVideoFrame MakeFrame(CPVideoFrame const& source) const;

};


/////////////////////////////////////////////////////////////////////////////
//  Tweak::YV12
//
//  Tweak subclass dealing with the YV12 color space
//
class Tweak::YV12 : public Tweak
{

public:  //constructors

  YV12(PClip child, double hue, double sat, double bright, double cont)
    : Tweak(child , hue, sat, bright, cont) { }

  YV12(PClip child, Tweak const& other)
    : Tweak(child, other) { }


protected:  //Pipeline MakeFrame method

  virtual CPVideoFrame MakeFrame(CPVideoFrame const& source) const;

};



} } //namespace avs::filters

#pragma warning ( pop )

#endif //__AVS_FILTERS_TWEAK_H__