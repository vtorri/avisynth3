// Avisynth v3.0 alpha.  Copyright 2004 Ben Rudiak-Gould et al.
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


#ifndef __AVS_CLIP_H__
#define __AVS_CLIP_H__

//avisynth include
#include "forward.h"

//boost include
#include <boost/enable_shared_from_this.hpp>


//definitely get rid of warning 4250 : MI dominance decisions
#pragma warning (disable : 4250)  


namespace avs {



///////////////////////////////////////////////////////////////////////////////
//  Clip
//
//  base class for all filters
//
class Clip : public boost::enable_shared_from_this<Clip>
{

public:  //structors

  Clip() { }
  virtual ~Clip() { }   //virtual destructor


public:  //clip general interface
  
  //get owning environment
  virtual PEnvironment const& GetEnvironment() const = 0;
  //get info about the clip
  virtual CPVideoInfo GetVideoInfo() const = 0;
 
  //get the frame n
  //undefined behavior if frame n don't exist
  virtual CPVideoFrame GetFrame(int n) const = 0;

  //fill the passed buffer with audio samples
  //out of bounds values are allowed, the excess is filled with blank noise
  virtual void GetAudio(void * buffer, __int64 start, int count) const = 0;  


public:  //filter chain simplication method

  //attempts to refactor self and its childs (if there is any)
  //into a more compact filter chain
  //a default version is provided (with no refactoring)
  virtual PClip Simplify() const { return shared_from_this(); }

};//Clip



} //namespace avs

#endif //__AVS_CLIP_H__