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



#ifndef  __SOURCE_H__
#define  __SOURCE_H__


#include <map>

#include "clip.h"
#include "cache.h"
#include "environment.h"

class ClipMaker;
class FoldingSourceClip;

typedef smart_ref<ClipMaker> PClipMaker;



class SourceClip : public Clip {

  PEnvironment env;

protected:
  SourceClip(PEnvironment _env) : env(_env) { }

public:
  PEnvironment GetEnvironment() const { return env; }

};



class ClipMaker : public RefCounted {

  typedef map<Clip *, Cache *> ClipToCacheMap;

  VideoInfo vi;
  ClipToCacheMap caches;                                                                                

  void RegisterClip(Clip * clip) { caches[clip] = new Cache(); }
  void UnregisterClip(Clip * clip) { delete caches[clip]; caches.erase(clip); }

  friend class FoldingSourceClip;     //so it can use the two above

public:
  ClipMaker() { }

  const VideoInfo& GetVideoInfo() const { return vi; }

  CPVideoFrame GetFrame(int n, Clip& client, Clip * source) { return caches[source]->GetFrame(n, client, *source); }

protected:
  virtual CPVideoFrame MakeFrame(int n, PEnvironment env) = 0;
};



class FoldingSourceClip : public SourceClip {

  PClipMaker maker;

public:
  FoldingSourceClip(PEnvironment env, PClipMaker _maker) : SourceClip(env), maker(_maker) { maker->RegisterClip(this); }

  virtual ~FoldingSourceClip() { maker->UnregisterClip(this); }

  virtual const VideoInfo& GetVideoInfo() const { return maker->GetVideoInfo(); }

  //classic GetFrame using cache
  virtual CPVideoFrame GetFrame(int n, Clip& client) { return cache.GetFrame(n, client, *this); }

protected:
  //forwards frame making request to maker
  virtual CPVideoFrame MakeFrame(int n) { return maker->GetFrame(n); }


};



class AviSourceClipMaker : public ClipMaker {



};

class AviSource : public FoldingSourceClip {


public:



};



#endif  //__SOURCE_H__