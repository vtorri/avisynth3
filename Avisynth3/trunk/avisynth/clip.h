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


#ifndef __CLIP_H__
#define __CLIP_H__


#pragma warning( disable : 4290 )  //get rid of C++ exception ... ignored warning

//stl includes
#include <string>      //std exceptions need that to be used
#include <utility>     //for pair
using namespace std;

//avisytnh includes
#include "refcounted.h" 

//class declarations
class Clip;
class Cache;
class VideoInfo;
class VideoFrame;
class ScriptEnvironment;

//typedefs
typedef unsigned char BYTE;
typedef smart_ref<Clip> PClip;
typedef smart_ref<ScriptEnvironment> PEnvironment;
typedef smart_ptr_to_cst<VideoFrame> CPVideoFrame;




/********************************************************************************************
 * Clip                                                                                     *
 ********************************************************************************************
  Base class for all filters.
*/



class Clip : public RefCounted {

public:
  Clip() { }
  
  //get owning environment
  virtual PEnvironment GetEnvironment() = 0;
  //get info about the clip
  virtual const VideoInfo& GetVideoInfo() = 0;
 
  //get the frame n for the passed client
  //try cache (if there is any) and forward to MakeFrame if not cached
  //frame n MUST exist (unexpected behavior otherwise)
  virtual CPVideoFrame GetFrame(int n, Clip& client) = 0;

  //request to fill the passed buffer with audio samples
  //out of bounds values are allowed, the excess is filled with blank noise
  virtual void GetAudio(BYTE * buf, __int64 start, __int64 count) = 0;  

protected:
  //process the frame n (without cache consideration)
  //default implementation: throw a logic_error exception
  //filters using cache should redefine it, others should never call it
  virtual CPVideoFrame MakeFrame(int n) { throw std::logic_error("Illegal MakeFrame Call"); }

  friend class Cache; //so it can use the above


  //another helper function, fill a buffer with blank noise (of the appropriate sample-type/channels)
  void FillWithBlank(BYTE * buf, __int64 count);

  //cache related code
  enum CacheMethod {
    CACHE_ALL,
    CACHE_NOTHING,
    CACHE_RANGE,
    CACHE_LAST
  };
  typedef pair<CacheMethod, unsigned> CachePolicy; 

  virtual CachePolicy GetWantedCachePolicy() { return make_pair(CACHE_NOTHING, 0); }
};





#endif //#ifndef __CLIP_H__