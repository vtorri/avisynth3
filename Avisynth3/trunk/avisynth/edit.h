// Avisynth v2.5.  Copyright 2002 Ben Rudiak-Gould et al.
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
//
// As a special exception, the copyright holders of Avisynth give you
// permission to link Avisynth with independent modules that communicate with
// Avisynth solely through the interfaces defined in avisynth.h, regardless of the license
// terms of these independent modules, and to copy and distribute the
// resulting combined work under terms of your choice, provided that
// every copy of the combined work is accompanied by a complete copy of
// the source code of Avisynth (the version of Avisynth used to produce the
// combined work), being distributed under the terms of the GNU General
// Public License plus this exception.  An independent module is a module
// which is not derived from or based on Avisynth, such as 3rd-party filters,
// import and export plugins, or graphical user interfaces.

#ifndef __Edit_H__
#define __Edit_H__

#include "internal.h"


/********************************************************************
********************************************************************/


/**
  * Class to select a range of frames from a longer clip
 **/
class Trim : public EditFilter {

  const int begin;
  const __int64 audio_offset;

public:
  //end is the out of bounds limit
  Trim(PClip  child, int _begin, int end) : ChildClip(child, child->GetVideoInfo().Trim(_begin, end)),
    begin(_begin), audio_offset(vi.AudioSamplesFromFrames(begin)) { }
     
  virtual CPVideoFrame GetFrame(int n, CLip& client) { return child->GetFrame(n + begin, client); }
  virtual void GetAudio(void* buf, __int64 start, __int64 count) { child->GetAudio( buf, start + audio_offset, + count); }

};




//common superclass for the two classes below
class DeleteDuplicateFunction : public CoreFilter {

  static const DescriptionPrototype prototype;  //(clip c, int n)

public:
  DeleteDuplicateFunction(const string& name) : CoreFilter(name) { }

  virtual const DescriptionPrototype& GetPrototype() const { return prototype; }


protected:
  virtual AVSValue operator() (const ArgVector& args, PEnvironment env) const { return Edit(any_cast<PClip>(args[0]), any_cast<int>(args[1]), env); }

  virtual PClip Edit(PClip clip, int n, PEnvironment env) const = 0;


};

class DeleteFrameFunction : public DeleteDuplicateFunction {

  DeleteFrameFunction instance;    

  //private constructor 
  DeleteFrameFunction() : DeleteDuplicateFunction("DeleteFrame") { }

protected:
  virtual PClip Edit(PClip clip, int n, PEnvironment env) const { return new Splice(new Trim(clip, 0, n), new Trim(clip, n+1, 0)); }
};


class DuplicateFrameFunction : public DeleteDuplicateFunction {

  DuplicateFrameFunction instance;

  //private constructor
  DuplicateFrameFunction() : DeleteDuplicateFunction("DuplicateFrame") { }

protected:
  virtual PClip Edit(PClip clip, int n, PEnvironment env) const { return new Splice(new Trim(clip, 0, n+1), new Trim(clip, n, 0)); }
};


/**
  * Class to display a single frame for the duration of several
 **/
class FreezeFrame : public ChildClip {

  const int first, last, source;

public:
  FreezeFrame(PClip _child, int _first, int _last, int _source)
    : ChildClip(child), first(_first), last(_last), source(_source) { }

  virtual CPVideoFrame GetFrame(int n, Clip& client) { return child->GetFrame((n >= first && n <= last) ? source : n, client); }

};





class Splice : public MultiChildClip {

  typedef vector<int> IntVector;
  
  const IntVector videoSwitchOvers;  

  //merge the videoinfos (and throw errors if not possible)
  static VideoInfo MergeVideoInfos(const ClipVector& childs);

  struct GetFrameCount {
    int operator()(const PClip& clip) const { return clip->GetVideoInfo().GetFrameCount(); }
  };

  static inline IntVector MakeSwitchOverVector(const ClipVector& childs)
  {
    IntVector result(childs.size() + 1, 0);  //create with good size
    transform(childs.begin(), childs.end(), result.begin() + 1, GetFrameCount());  //fill with frame count of each clip
    partial_sum(result.begin(), result.end(), result.begin());                 //partial sum
    return result;
  }

public:
  Splice(const ClipVector& _childs) : MultiChildClip(_childs, MergeVideoInfos(_childs)),
    videoSwitchOvers(MakeSwitchOverVector(_childs)) { }

  virtual CPVideoFrame GetFrame(int n, Clip& client)
  {
    IntVector::const_iterator it = upper_bound(videoSwitchOvers.begin(), videoSwitchOvers.end(), n);
    //if == begin it's < 0 and if == end it's >= framecount
    if ( it == videoSwitchOvers.begin() || it == videoSwitchOvers.end() )
      ThrowOutOfBoundsException();
    return GetChildFrame(it -  videoSwitchOvers.begin(), n - *it);
  }

};





class Dissolve : public GenericVideoFilter 
/**
  * Class to smoothly transition from one video clip to another
 **/
{
public:
  Dissolve(PClip _child1, PClip _child2, int _overlap, IScriptEnvironment* env);
  PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
  void __stdcall GetAudio(void* buf, __int64 start, __int64 count, IScriptEnvironment* env);
  bool __stdcall GetParity(int n);

  static AVSValue __cdecl Create(AVSValue args, void*, IScriptEnvironment* env);

private:
  const PClip child2;
  const int overlap;
  int video_fade_start, video_fade_end;
  int audio_fade_start, audio_fade_end;
  BYTE* audbuffer;
  int audbufsize;
  void EnsureBuffer(int minsize);
};




class AudioDub : public IClip {  
/**
  * Class to mux the audio track of one clip with the video of another
 **/
public:
  AudioDub(PClip child1, PClip child2, IScriptEnvironment* env);
  const VideoInfo& __stdcall GetVideoInfo();
  PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
  void __stdcall GetAudio(void* buf, __int64 start, __int64 count, IScriptEnvironment* env);
  bool __stdcall GetParity(int n);
  void __stdcall SetCacheHints(int cachehints,int frame_range) { };

  static AVSValue __cdecl Create(AVSValue args, void*, IScriptEnvironment* env);

private:
  /*const*/ PClip vchild, achild;
  VideoInfo vi;
};




class Reverse : public GenericVideoFilter 
/**
  * Class to play a clip backwards
 **/
{
public:
  Reverse(PClip _child);
  PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
  bool __stdcall GetParity(int n);
  void __stdcall GetAudio(void* buf, __int64 start, __int64 count, IScriptEnvironment* env);

  static AVSValue __cdecl Create(AVSValue args, void*, IScriptEnvironment* env);
};



class AudioLoop : public EditFilter {
  
  const int times;
  const __int64 audioModulo;

protected:
  AudioLoop(PClip child, int _times, __int64 _audioModulo) : EditFilter(child, child->GetVideoInfo().Loop(_times),
    times(_times), audioModulo(_audioModulo) { }

public:
  virtual void GetAudio(BYTE * buf, __int64 start, __int64 count)
  {
    //FIXME : take care of the case start/count out of bounds

    start = start % audioModulo;
    if ( start + count < audioModulo )     //no overlap    
      child->GetAudio(buf, start, count);  //we just forward request
    else {
      child->GetAudio(buf, start, audioModulo - start);  //we fetch the first part

      //update variables
      count -= audioModulo - start;   //samples left to fetch
      int grain = GetVideoInfo().GetAudioGrain();
      buf += grain * (audioModulo - start);

      //a loop to get all of the complete audio length we can
      for(int i = count/audioModulo; i-- > 0; buf += grain * audioModulo)
        child->GetAudio(buf, 0, audioModulo);

      child->GetAudio(buf, 0, count % audioModulo);   //fetch the final part
    }       
  }

};

class Loop : public AudioLoop {

  const int videoModulo;

public:
  Loop(PClip child, int times) : AudioLoop(child, times, child->GetVideoInfo().GetSamplesFromFrameCount()),
    videoModulo(child->GetVideoInfo().GetFrameCount()) { }

  virtual CPVideoFrame GetFrame(int n, Clip& client) { CheckFrameInBounds(n); return GetChildFrame(n % videoModulo); }
};


class Loop : public GenericVideoFilter {
/**
  * Class to loop over a range of frames
**/
public:
	Loop(PClip _child, int times, int _start, int _end, IScriptEnvironment* env);
	PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
	bool __stdcall GetParity(int n);
  void __stdcall GetAudio(void* buf, __int64 start, __int64 count, IScriptEnvironment* env);

	static AVSValue __cdecl Create(AVSValue args, void*, IScriptEnvironment* env);
private:
	int frames, start, end, times;
  __int64 start_samples, loop_ends_at_sample,loop_len_samples;
	int convert(int n);
};




/**** A few factory methods ****/

static AVSValue __cdecl Create_FadeOut(AVSValue args, void*, IScriptEnvironment* env);
static AVSValue __cdecl Create_FadeOut2(AVSValue args, void*, IScriptEnvironment* env);
static AVSValue __cdecl Create_FadeIn(AVSValue args, void*, IScriptEnvironment* env);
static AVSValue __cdecl Create_FadeIn2(AVSValue args, void*,IScriptEnvironment* env);
static AVSValue __cdecl Create_FadeIO(AVSValue args, void*,IScriptEnvironment* env);
static AVSValue __cdecl Create_FadeIO2(AVSValue args, void*,IScriptEnvironment* env);

PClip new_Splice(PClip _child1, PClip _child2, bool realign_sound, IScriptEnvironment* env);


#endif  // __Edit_H__