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


#ifndef __AVS_ABSTRACTSPLICE_H__
#define __AVS_ABSTRACTSPLICE_H__


//avisynth include
#include "../videoinfoclip.h"

//stl include
#include <vector>



/////////////////////////////////////////////////////////////////////////////////////
//  AbstractSplice
//
//  common superclass factorising code for Splice and UnalignedSplice
//
class AbstractSplice : public VideoInfoClip
{

private:  //typedefs

  typedef std::vector<PClip> ClipVector;
  typedef std::vector<int> IntVector;
  typedef std::vector<__int64> Int64Vector;


private:  //members

  ClipVector childs_;     //childs vector
  IntVector video_sw;     //video switch over points
  Int64Vector audio_sw;   //audio switch over points

  PEnvironment env_;      //owning env
  

public:  //constructors

  AbstractSplice(const VideoInfo& vi, PEnvironment env)
    : VideoInfoClip( vi.MakeNull() )
    , env_( env )
  {
    video_sw.push_back(0);
    audio_sw.push_back(0);
  }


public:  //clip general interface

  virtual PEnvironment GetEnvironment() const { return env_; }

  virtual CPVideoFrame GetFrame(int n, const CachingClip& client) const;
  virtual void GetAudio(BYTE * buf, __int64 start, __int64 count) const;


public:  //read access

  const ClipVector& GetChilds() const { return childs_; }
  int ChildCount() const { return childs_.size(); }


public:  //write access

  void AddChild(PClip child);


private:  

  virtual const AbstractSplice * IsAbsorbable(const Clip * child) const = 0;

  //called only when vi has both video and audio (so synch is an issue)
  virtual __int64 SamplesToSpliceFrom(const VideoInfo& vi) const = 0;

};


#endif //__AVS_ABSTRACTSPLICE_H__