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


#ifndef __AVS_VIDEOINFO_H__
#define __AVS_VIDEOINFO_H__

//avisynth includes
#include "forward.h"
#include "sampletype.h"


namespace avs {



///////////////////////////////////////////////////////////////////////////////
//  VideoInfo
//
//  The VideoInfo class holds global information about a clip 
//  (i.e. information that does not depend on the frame number).  
//  The GetVideoInfo method in Clip returns this class.
//
//  As of 3.0 VideoInfo becomes a polymorphic class 
//  which is responsible of checking all clip constraints.
//  ie all tests are the responsability of VideoInfo,
//  you try it and if illegal it will throw the appropriate error.
//
class VideoInfo
{
  
public:  //structors

  VideoInfo() { }
  virtual ~VideoInfo() { }


public:  //clone method

  virtual CPVideoInfo clone() const = 0;


public:  //factory method

  static CPVideoInfo Create();


public:  //video methods

  virtual bool HasVideo() const = 0;

  virtual ColorSpace& GetColorSpace() const = 0;
  virtual Dimension const& GetDimension() const = 0;
  virtual int GetFrameCount() const = 0;
  virtual Fraction const& GetFPS() const = 0;
  virtual bool IsFrameClip() const = 0;

  virtual int GetWidth() const;
  virtual int GetHeight() const;

  int GetFPSNumerator() const { return GetFPS().numerator(); }
  int GetFPSDenominator() const { return GetFPS().denominator(); }

  //write access

  virtual void SetColorSpace(ColorSpace& space) = 0;
  virtual void SetDimension(Dimension const& dim) = 0;
  virtual void SetFrameCount(int frameCount) = 0;
  virtual void SetFPS(Fraction const& fps) = 0;
  virtual void SetIsFrameClip(bool frameClip) = 0;

  virtual void SetWidth(int width);
  virtual void SetHeight(int height);

  void SetFPS(int numerator, int denominator) { SetFPS( Fraction(numerator, denominator) ); }

  void AddToFrameCount(int shift) { SetFrameCount(GetFrameCount() + shift); }

  //various queries
 
  virtual bool IsRGB() const;
  virtual bool IsYUV() const;
  virtual bool IsPlanar() const;
  virtual bool IsInterLeaved() const;

  virtual bool IsColorSpace(ColorSpace& space);
  
  virtual bool IsRGB24() const;
  virtual bool IsRGB32() const;
  virtual bool IsRGB45() const;
  virtual bool IsYUY2() const;
  virtual bool IsYV12() const;
  virtual bool IsYV24() const;
  virtual bool IsYV45() const;


  //virtual void AddVideo();
  virtual void AddVideo(ColorSpace& space, Dimension const& dim, int frameCount, Fraction const& fps = 25, bool frameClip = true) = 0;

  virtual void KillVideo() = 0;


public:  //audio methods

  virtual bool HasAudio() const = 0;

  virtual SampleType GetSampleType() const = 0;
  virtual int GetSampleRate() const = 0;
  virtual int64 GetSampleCount() const = 0;
  virtual int GetChannelCount() const = 0;

  //write access

  virtual void SetSampleType(SampleType sampleType) = 0;
  virtual void SampleRate(int sampleRate) = 0;
  virtual void SetSampleCount(int64 sampleCount) = 0;
  virtual void SetChannelCount(int channelCount) = 0;

  void AddToSampleCount(int64 shift) { SetSampleCount(GetSampleCount() + shift); }


  //virtual void AddAudio();
  virtual void AddAudio(SampleType sampleType, int sampleRate, int64 sampleCount, int channelCount) = 0;

  virtual void KillAudio() = 0;

  virtual void MergeAudio(VideoInfo const& other) = 0;


public:  

  virtual int BitsPerPixel() const;

  int BytesPerAudioSample() const { return BytesPerChannelSample() * GetChannelCount(); }
  virtual int BytesPerChannelSample() const;

  virtual int AudioSamplesFromFrames(int frames) const;


public:  //various check methods (failure means proper exception)

  virtual void CheckHasVideo() const;
  virtual void CheckHasAudio() const;

  virtual void CheckHasFrame(int n) const;

};//VideoInfo



} //namespace avs

#endif  //#ifndef __AVS_VIDEOINFO_H__
