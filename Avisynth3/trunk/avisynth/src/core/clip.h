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


//boost include
#include <boost/shared_ptr.hpp>


namespace avs {


//class declarations
class Clip;
class VideoInfo;
class VideoFrame;
class RuntimeEnvironment;

//typedefs
typedef boost::shared_ptr<Clip const> PClip;
typedef boost::shared_ptr<VideoFrame const> CPVideoFrame;
typedef boost::shared_ptr<RuntimeEnvironment> PEnvironment;




///////////////////////////////////////////////////////////////////////////////
// Clip
//
// base class for all filters
//
class Clip
{

public:  //structors

  Clip() { }
  virtual ~Clip() { }   //virtual destructor


public:  //clip general interface
  
  //get owning environment
  virtual PEnvironment GetEnvironment() const = 0;
  //get info about the clip
  virtual VideoInfo const& GetVideoInfo() const = 0;
 
  //get the frame n
  //undefined behavior if frame n don't exist
  virtual CPVideoFrame GetFrame(int n) const = 0;

  //fill the passed buffer with audio samples
  //out of bounds values are allowed, the excess is filled with blank noise
  virtual void GetAudio(void * buffer, __int64 start, int count) const = 0;  


protected:  //filter chain simplication method

  //attempts to refactor self and its childs (if there is any)
  //into a more compact filter chain
  //a default version is provided (with no refactoring)
  //
  //NB: this method must be called like this:
  //    PClip clip = ...
  //    PClip simplified = clip->Simplify(clip);
  virtual PClip Simplify(PClip self) const { return self; }


public:

  //provides correct call to the above method
  static PClip SimplifyClip(PClip clip) { return clip->Simplify(clip); }


protected:  //implementation helpers

  //fill a buffer with blank noise (of the appropriate sample-type/channels)
  void FillWithBlank(BYTE * buffer, int count) const;

  void ThrowNoSuchFrameException(int n) const;
  void ThrowNoAudioException() const;

};//Clip



} //namespace avs

#endif //#ifndef __AVS_CLIP_H__