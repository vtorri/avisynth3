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

#include "error.h"  //which includes <string>
#include "videoproperty.h"



// The VideoInfo class holds global information about a clip 
// (i.e. information that does not depend on the frame number).  
// The GetVideoInfo method in IClip returns this class.

// As of 3.0 VideoInfo becomes a class and is now responsible of checking all clip constraints.
// (ie no test anything anymore: you try it and if illegal it will throw the approprite error)
// All mutating methods return either VideoInfo& or VideoInfo in order to allow chaining 
// (

// Audio Sample information
typedef float SFLOAT;

class VideoInfo {

public:

  typedef short dimension;
  
  
  //some enumerations
  enum SampleType {
    SAMPLE_INT8,
    SAMPLE_INT16, 
    SAMPLE_INT24,    // Int24 is a very stupid thing to code, but it's supported by some hardware.
    SAMPLE_INT32,
    SAMPLE_FLOAT
  };  



  //default constructor : no video, no audio
  VideoInfo();
  VideoInfo(const VideoInfo& other);  //copy constructor


  /*
   * Video stuff
   */
private:


  dimension width, height;    
  unsigned fps_numerator, fps_denominator;
  int frameCount;

  void CheckHasVideo() const {
    if (! HasVideo())
      throw InternalError("VideoInfo: Invalid State: no Video");
  }

public:


  bool HasVideo() const;

  bool IsColorSpace(const ColorSpace& space) const { return GetColorSpace() == space; } 
  bool HasProperty(ColorSpace::Property prop) const { return GetColorSpace().HasProperty(prop); }

  bool IsPlanar() const { return HasProperty(ColorSpace::PLANAR); }
 
  bool IsRGB() const { return HasProperty(ColorSpace::RGB); }      
  bool IsRGB24() const { return IsColorSpace(RGB24::instance); } 
  bool IsRGB32() const { return IsColorSpace(RGB32::instance); }

  bool IsYUV() const { return HasProperty(ColorSpace::YUV); }
  bool IsYUY2() const { return IsColorSpace(YUY2::instance); }  
  bool IsYV12() const { return IsColorSpace(YV12::instance); }

  bool IsVPlaneFirst() const { return IsYV12(); }  // Don't use this 


  dimension GetWidth() const { CheckHasVideo(); return width; }
  dimension GetHeight() const { CheckHasVideo(); return height; }
  int GetFrameCount() const { CheckHasVideo(); return frameCount; }

  unsigned GetFPSNumerator() const { CheckHasVideo(); return fps_numerator; }
  unsigned GetFPSDenominator() const { CheckHasVideo(); return fps_denominator; }
  double GetFPS() const { CheckHasVideo(); return fps_numerator/fps_denominator; }

  const ColorSpace& GetColorSpace() const;

  VideoInfo& RemoveVideo();
  VideoInfo  RemoveVideo() const; 

  VideoInfo& SetColorSpace(const ColorSpace& space);
  VideoInfo  SetColorSpace(const ColorSpace& space) const;

  VideoInfo& SetWidth(dimension _width);
  VideoInfo  SetWidth(dimension _width) const;
  VideoInfo& SetHeight(dimension _height);
  VideoInfo  SetHeight(dimension _height) const;

  VideoInfo& Resize(dimension left, dimension right, dimension top, dimension bottom);
  VideoInfo  Resize(dimension left, dimension right, dimension top, dimension bottom) const { return VideoInfo(*this).Resize(left, right, top, bottom); }

  VideoInfo& SetFrameCount(int _frameCount) {
    if (_frameCount <= 0)
      throw InternalError("Attempted to request a Clip with negative or zero VideoFrame");
    frameCount = _frameCount;
    return *this;
  }
  VideoInfo  SetFrameCount(int _frameCount) const { return VideoInfo(*this).SetFrameCount(_frameCount); }

  VideoInfo& SetFPS(unsigned numerator, unsigned denominator) {
    CheckHasVideo();
    if (denominator == 0)
      throw InternalError("Attempted to set an infinite fps");
    unsigned x = numerator, y = denominator;
    while (y) {   // find gcd
      unsigned t = x%y; x = y; y = t;
    }
    fps_numerator = numerator/x;
    fps_denominator = denominator/x;
    return *this;
  }
  VideoInfo  SetFPS(unsigned numerator, unsigned denominator) const { return VideoInfo(*this).SetFPS(numerator, denominator); }




  /*
   * Audio stuff
   */
private:

  SampleType sample_type;           
  int audio_samples_per_second;   // 0 means no audio
  __int64 num_audio_samples;      // changed as of 2.5
  int nchannels;                  // as of 2.5

  void CheckHasAudio() const {
    if (! HasAudio())
      throw InternalError("VideoInfo: Invalid State: no audio");
  }

public:

  bool HasAudio() const { return audio_samples_per_second != 0; }
  void RemoveAudio() { audio_samples_per_second = 0; }

  SampleType GetSampleType() const { return sample_type; }
  int GetChannelCount() const { return nchannels; }
  int GetSamplesPerSecond() const { return audio_samples_per_second; }
  __int64 GetSamplesCount() const { return num_audio_samples; }

  void SetAudio(SampleType _sample_type, int _nchannels, int samples_per_sec, __int64 samples_count) {
    if (_nchannels <= 0 || samples_per_sec < 0 || samples_count < 0)
      throw AvisynthError("Filter Error: Requested an illegal VideoInfo");
    sample_type = _sample_type;
    nchannels = nchannels;
    audio_samples_per_second = samples_per_sec;
    num_audio_samples = samples_count;
  }

  int FramesFromAudioSamples(__int64 samples) const { return (int)(samples * (__int64)fps_numerator / (__int64)fps_denominator / (__int64)audio_samples_per_second); }
  __int64 AudioSamplesFromFrames(__int64 frames) const { return (__int64(frames) * audio_samples_per_second * fps_denominator / fps_numerator); }
  __int64 AudioSamplesFromBytes(__int64 bytes) const { return bytes / BytesPerAudioSample(); }
  __int64 BytesFromAudioSamples(__int64 samples) const { return samples * BytesPerAudioSample(); }

  int BytesPerAudioSample() const { return nchannels * BytesPerChannelSample();}
  int BytesPerChannelSample() const { 
    static int bytes[] = { sizeof(signed char), sizeof(signed short), 3, sizeof(signed int), sizeof(SFLOAT) };
    return bytes[sample_type];
  }


 
};

#endif  //#ifndef __VIDEOINFO_H__