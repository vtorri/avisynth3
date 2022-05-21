// Avisynth v3.0 alpha.  Copyright 2003-2005 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_FILTERS_STACK_H__
#define __AVS_FILTERS_STACK_H__

//avisynth includes
#include "../../core/geometry/vecteur.h"
#include "../../clip/twochilds/concrete.h"
#include "../../clip/framemaker/concrete.h"


namespace avs { namespace filters { 



/////////////////////////////////////////////////////////////////////////////////////////
//  Stack
//
//  factorisation superclass for Stack::Horizontal and Stack::Vertical
//  
class Stack : public clip::twochilds::Concrete
            , public clip::framemaker::Concrete
{
    
  CPVideoInfo vi_;

  
public:  //structors

  Stack(PClip const& first, PClip const& second, Dimension const& expand);

  //generated destructor is fine


public:  //clip general interface
    
  virtual CPVideoInfo GetVideoInfo() const { return vi_; }

  virtual BYTE * GetAudio(BYTE * buffer, long long start, long count) const;


private:  //FrameMaker interface

  virtual CPVideoFrame operator()(long n) const;


private:  //Stack interface

  virtual Vecteur GetShiftVecteur() const = 0;
    
};



} } //namespace avs::filters

#endif //__AVS_FILTERS_STACK_H__
