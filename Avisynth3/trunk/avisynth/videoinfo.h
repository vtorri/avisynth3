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


#ifndef __VIDEOINFO_H__
#define __VIDEOINFO_H__

#include "videoproperties.h"  //which includes colorspace.h, refcounted.h




enum SampleType {
  SAMPLE_INT8,
  SAMPLE_INT16, 
  SAMPLE_INT24,    // Int24 is a very stupid thing to code, but it's supported by some hardware.
  SAMPLE_INT32,
  SAMPLE_FLOAT
};  

class ClipAudioProperties : public RefCounted {

  SampleType sampleType;       
  int samplesPerSecond;   
  __int64 samplesCount;      
  int channelCount;        
  
  static __int64 CheckSamplesCount(__int64 samplesCount)
  {
    if (samplesCount <= 0)
      throw invalid_argument("Samples Count must be strictly positive");
    return samplesCount;
  }
  static int CheckSign(int value, const string& err_msg)
  {
    if (value <= 0)
      throw invalid_argument(err_msg);
    return value;
  }

public:
  ClipAudioProperties(SampleType _sampleType, __int64 _samplesCount) : sampleType(_sampleType), 
    samplesPerSecond(44100), samplesCount(CheckSamplesCount(_samplesCount)), channelCount(2) { }
  ClipAudioProperties(const ClipAudioProperties& other) : sampleType(other.sampleType),
    samplesPerSecond(other.samplesPerSecond), samplesCount(other.samplesCount), channelCount(other.channelCount) { }

  SampleType GetSampleType() const { return sampleType; }
  int GetSamplesPerSecond() const { return samplesPerSecond; }
  __int64 GetSamplesCount() const { return samplesCount; }
  int GetChannelCount() const { return channelCount; }

  void SetSampleType(SampleType _sampleType) { sampleType = _sampleType; }
  void SetSamplesPerSecond(int _samplesPerSecond) { samplesPerSecond = CheckSign(_samplesPerSecond, "Samples per second must be strictly positive"); }
  void SetSamplesCount(__int64 _samplesCount) { samplesCount = CheckSamplesCount(_samplesCount); }
  void SetChannelCount(int _channelCount) { channelCount = CheckSign(_channelCount, "Channel count must be strictly positive"); }

  ClipAudioProperties * clone() const { return new ClipAudioProperties(*this); }
};

typedef smart_ptr<ClipAudioProperties> PClipAudProps;



// The VideoInfo class holds global information about a clip 
// (i.e. information that does not depend on the frame number).  
// The GetVideoInfo method in Clip returns this class.

// As of 3.0 VideoInfo becomes a class and is now responsible of checking all clip constraints.
// ie all tests are the responsability of VideoInfo, you try it and if illegal it will throw the appropriate error)
// All mutating methods return VideoInfo& in order to allow chaining 


// Audio Sample information
typedef float SFLOAT;


class ScriptEnvironment;
typedef smart_ptr<ScriptEnvironment> PEnvironment;

class VideoInfo {
  
  PClipVidProps vidProps;  //NULL if no video else valid
  PClipAudProps audProps;  //NULL if no audio else valid
  PEnvironment env;

public:
  //default constructor : no video, no audio
  VideoInfo(PEnvironment _env) : env(_env) { }
  //copy constructor
  VideoInfo(const VideoInfo& other) : vidProps(other.vidProps), audProps(other.audProps), env(other.env) { }
  
  PEnvironment GetEnvironment() const { return env; }

  bool IsNullClip() const
  {
    return ( ! HasVideo() || GetFrameCount() == 0 )        //if has video no frames   
        && ( ! HasAudio() || GetSamplesCount() == 0 );     //if has audio no samples
  }

  void SetNullClip()
  {
    if ( HasVideo() )
      SetFrameCount(0);
    if ( HasAudio() )
      SetSamplesCount(0);
  }

  /*
   * Video stuff
   */ 
private:  
  void CheckHasVideo() const
  {
    if (! HasVideo())
      throw logic_error("VideoInfo: Invalid State: no Video");
  }

  ClipVideoProperties& ChangeVideoProperties()
  { 
    CheckHasVideo();
    vidProps.UnShare(); 
    return *vidProps;
  }

public:
  bool HasVideo() const { return vidProps; }

  //use this one if you have many info requests
  const ClipVideoProperties& GetVideoProperties() const { CheckHasVideo(); return *vidProps; }

  const ColorSpace& GetColorSpace() const { return GetVideoProperties().GetColorSpace(); }
  const Dimension& GetDimension() const { return GetVideoProperties().GetDimension(); }
  int GetWidth() const { return GetVideoProperties().GetWidth(); }
  int GetHeight() const { return GetVideoProperties().GetHeight(); }
  bool IsFrameClip() const { return GetVideoProperties().IsFrameClip(); }
  const Fraction& GetFPS() const { return GetVideoProperties().GetFPS(); }
  int GetFrameCount() const { return GetVideoProperties().GetFrameCount(); }


  bool IsColorSpace(const ColorSpace& space) const { return GetColorSpace() == space; } 
  bool HasProperty(ColorSpace::Property prop) const { return GetColorSpace().HasProperty(prop); }

  bool IsPlanar() const { return HasProperty(ColorSpace::PLANAR); }
 
  bool IsRGB() const { return HasProperty(ColorSpace::RGB); }      
  bool IsRGB24() const { return IsColorSpace(RGB24::instance); } 
  bool IsRGB32() const { return IsColorSpace(RGB32::instance); }

  bool IsYUV() const { return HasProperty(ColorSpace::YUV); }
  bool IsYUY2() const { return IsColorSpace(YUY2::instance); }  
  bool IsYV12() const { return IsColorSpace(YV12::instance); }


  VideoInfo& KillVideo() { vidProps = NULL; return *this; }

  VideoInfo& AddVideo(const ColorSpace& space, const Dimension& dimension) { vidProps = new ClipVideoProperties(space, dimension); return *this; }

  VideoInfo& SetColorSpace(const ColorSpace& space) { ChangeVideoProperties().SetColorSpace(space); return *this; }
  VideoInfo& SetDimension(const Dimension& dimension) { ChangeVideoProperties().SetDimension(dimension); return *this; }
  VideoInfo& SetWidth(int width) { ChangeVideoProperties().SetWidth(width); return *this; }
  VideoInfo& SetHeight(int height) { ChangeVideoProperties().SetHeight(height); return *this; }
  VideoInfo& SetFrameClip(bool frameClip) { ChangeVideoProperties().SetFrameClip(frameClip); return *this; }
  VideoInfo& SetFPS(const Fraction& fps) { ChangeVideoProperties().SetFPS(fps); return *this; }
  VideoInfo& SetFrameCount(int frameCount) { ChangeVideoProperties().SetFrameCount(frameCount); return *this; }



  /*
   * Audio stuff
   */ 
private:
  void CheckHasAudio() const
  {
    if (! HasAudio())
      throw logic_error("VideoInfo: Invalid State: no audio");
  }

  ClipAudioProperties& ChangeAudioProperties()
  { 
    CheckHasAudio();
    audProps.UnShare(); 
    return *audProps;
  }

public:

  bool HasAudio() const { return audProps; }

  const ClipAudioProperties& GetAudioProperties() const { CheckHasAudio(); return *audProps; }

  SampleType GetSampleType() const { return GetAudioProperties().GetSampleType(); }
  int GetSamplesPerSecond() const { return GetAudioProperties().GetSamplesPerSecond(); }
  __int64 GetSamplesCount() const { return GetAudioProperties().GetSamplesCount(); }
  int GetChannelCount() const { return GetAudioProperties().GetChannelCount(); }


  VideoInfo& KillAudio() { audProps = NULL; return *this; }

  VideoInfo& AddAudio(SampleType sampleType, __int64 samplesCount) { audProps = new ClipAudioProperties(sampleType, samplesCount); return *this; }

  VideoInfo& SetSampleType(SampleType sampleType) { ChangeAudioProperties().SetSampleType(sampleType); return *this; }
  VideoInfo& SetSamplesPerSecond(int samplesPerSecond) { ChangeAudioProperties().SetSamplesPerSecond(samplesPerSecond); return *this; }
  VideoInfo& SetSamplesCount(__int64 samplesCount) { ChangeAudioProperties().SetSamplesCount(samplesCount); return *this; }
  VideoInfo& SetChannelCount(int channelCount) { ChangeAudioProperties().SetChannelCount(channelCount); return *this; }



};





#endif  //#ifndef __VIDEOINFO_H__