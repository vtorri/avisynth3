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


#ifndef __AVS_CLIP_H__
#define __AVS_CLIP_H__

//avisynth includes
#include "forward.h"
#include "../define.h"    //for AVS_NOVTABLE macro

//boost include
#include <boost/enable_shared_from_this.hpp>


#ifdef _MSC_VER
//definitely get rid of warning 4250 : MI dominance decisions
#pragma warning (disable : 4250)  
#endif //_MSC_VER



namespace avs {



///////////////////////////////////////////////////////////////////////////////
//  Clip
//
//  base class for all filters
//
class AVS_NOVTABLE Clip : public boost::enable_shared_from_this<Clip>
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
  virtual CPVideoFrame GetFrame(long n) const = 0;

  //fill the passed buffer with audio samples, and returns position after them
  //out of bounds values are allowed, the excess is filled with blank noise
  virtual BYTE * GetAudio(BYTE * buffer, long long start, long count) const = 0;  


public:  //filter chain simplication methods

  //attempts to refactor self and its childs (if there is any)
  //into a more compact filter chain
  //a default version is provided (with no refactoring)
  virtual PClip Simplify() const { return shared_from_this(); }


  virtual PClip FinalSimplify() const { return shared_from_this(); }

};//Clip



} //namespace avs

#endif //__AVS_CLIP_H__
