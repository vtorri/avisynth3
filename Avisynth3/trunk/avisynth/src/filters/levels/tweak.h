// Avisynth v3.0 alpha.  Copyright 2005 David Pierre - Ben Rudiak-Gould et al.
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

//avisynth includes
#include "../../clip/onechild/concrete.h"
#include "../../clip/framemaker/concrete.h"
#include "../../clip/onechild/framemakerpipeline.h"


namespace avs { namespace filters {



/////////////////////////////////////////////////////////////////////////////
//  Tweak
//
//
//
class Tweak : public clip::onechild::FrameMakerPipeline
            , public clip::onechild::Concrete  
            , public clip::framemaker::Concrete
{

protected:  //members

  int Sin, Cos;         // = (int) sin, cos  ( (hue * 3.1415926) / 180.0 ) * 4096
  int Sat;              // = (int) sat * 512
  int Bright_p16;       // = (int) bright + 16        
  int Cont;             // = (int) cont * 512


public:  //constructors

  Tweak(PClip const& child, double hue, double sat, double bright, double cont);

  Tweak(PClip const& child, Tweak const& other);


public:  //Clip general interface

  virtual CPVideoInfo GetVideoInfo() const { return GetChildVideoInfo(); }

  virtual BYTE * GetAudio(BYTE * buffer, long long start, long count) const { return GetChildAudio(buffer, start, count); }


public:  //child changing clone

  virtual PClip clone(PClip const& child) const;


public:  //factory method 

  static PClip Create(PClip const& child, double hue, double sat, double bright, double cont);

};

  



} } //namespace avs::filters

#endif //__AVS_FILTERS_TWEAK_H__
