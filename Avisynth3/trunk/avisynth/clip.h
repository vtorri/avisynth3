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


#ifndef __AVS_CLIP_H__
#define __AVS_CLIP_H__


//avisytnh includes
#include "smart_ref.h"

//boost includes
#include <boost/tuple/tuple.hpp>  //for tuples


//class declarations
class Clip;
class VideoInfo;
class VideoFrame;
class CachingClip;
class RuntimeEnvironment;


//typedefs
typedef unsigned char BYTE;
typedef smart_ref<const Clip> PClip;
typedef smart_ref<const VideoFrame> CPVideoFrame;
typedef smart_ref<RuntimeEnvironment> PEnvironment;




///////////////////////////////////////////////////////////////////////////////
// Clip
//
// base class for all filters
//
class Clip
{

public:  //enums and typedefs

  enum CacheMethod
  {
    CACHE_ALL,
    CACHE_NOTHING,
    CACHE_RANGE,
    CACHE_LAST
  };

  typedef boost::tuples::tuple<CacheMethod, unsigned> CachePolicy;
  typedef boost::tuples::tuple<const CachingClip *, CacheMethod, unsigned> CacheRequest;


public:  //structors

  Clip() { }
  virtual ~Clip() { }   //virtual destructor


public:  //clip general interface
  
  //get owning environment
  virtual PEnvironment GetEnvironment() const = 0;
  //get info about the clip
  virtual const VideoInfo& GetVideoInfo() const = 0;
 
  //get the frame n
  //undefined behavior if frame n don't exist
  virtual CPVideoFrame GetFrame(int n, const CachingClip& client) const = 0;

  //fill the passed buffer with audio samples
  //out of bounds values are allowed, the excess is filled with blank noise
  virtual void GetAudio(BYTE * buf, __int64 start, __int64 count) const = 0;  


public:  //cache hints methods

  virtual void Dispatch(const CacheRequest& request) const = 0;
  virtual void Withdraw(const CacheRequest& request) const = 0;


public:

  //filter chain simplication method
  //attempts to refactor self and its childs (if there is any)
  //into a more compact filter chain
  //a default version is provided (with no refactoring)
  //
  //NB: this method must be called like this:
  //    PClip clip = ...
  //    PClip simplified = clip->Simplify(clip);
  virtual PClip Simplify(PClip self) const { return self; }


protected:  //implementation helpers

  //fill a buffer with blank noise (of the appropriate sample-type/channels)
  void FillWithBlank(BYTE * buf, __int64 count) const;

  void ThrowNoSuchFrameException(int n) const;

};





#endif //#ifndef __AVS_CLIP_H__