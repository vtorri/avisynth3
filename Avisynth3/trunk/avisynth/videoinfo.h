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


class VideoInfo;

//ColorSpace ids
enum CS_ID {
  I_VOID,
  I_BRG24,
  I_BGR32,
  I_YUY2,
  I_YV12,
  I_I420
};

class ColorSpace {

  static const int flagList[];  //used to associate flags to id

  ColorSpace(CS_ID _id) : id(_id), flags(flagList[id]) { } 
  friend VideoInfo; //only VideoInfo can construct them

  const int flags; 
  //properties flags, it's private so you would use HasFlag
  //then if properties grow to get over 32 we can simply change it as __int64 or an array

public:
  const CS_ID id;   //used to switch on ColorSpaces 

  //ColorSpace properties
  enum Property {
    BGR,
    YUV,
    INTERLEAVED,
    PLANAR,
  };

  bool operator== (const ColorSpace& other) const { return id == other.id; }
  bool operator!= (const ColorSpace& other) const { return id != other.id; }
  bool operator< (const ColorSpace& other) const { return id < other.id; } //may (or may not) be useful

  bool HasFlag(Property prop) const { return (flags & (1<<prop)) != 0; }
};


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

  //ColorSpace static consts
  //those are the only ColorSpace instances that may exist
private:
  static const ColorSpace NO_VIDEO;  //reserved for internal use
public:
  static const ColorSpace CS_BGR24;
  static const ColorSpace CS_BGR32;
  static const ColorSpace CS_YUY2;
  static const ColorSpace CS_YV12;
  static const ColorSpace CS_I420;




  //methods used to check that clip dimensions respect ColorSpace contraints  
  //for convenience reasons, they return their argument  
  static inline dimension WidthParityCheck(dimension width, const ColorSpace& space) {
    switch(space.id)
    {
      case I_YUY2:
        if ( width & 1)
          throw InternalError("Attempted to request a YUY2 VideoFrame with an odd width");
        break;
      case I_YV12:
      case I_I420:
        if ( width & 1)
          throw InternalError("Attempted to request a YV12 VideoFrame with an odd width");

    }
    return width;
  }
  static inline WidthCheck(dimension width, const ColorSpace& space)
  {
    if (width <= 0)
      throw InternalError("Attempted to request a zero width VideoFrame");
    return WidthParityCheck(width, space);
  }

  static inline dimension HeightParityCheck(dimension height, bool fieldBased, const ColorSpace& space) {
    switch(space.id)
    {
      case I_YV12:
      case I_I420:
        if (height & 1)
          throw InternalError("Attempted to request a YV12 VideoFrame with an odd height");
        if (fieldBased && (height & 3))
          throw InternalError("Attempted to request a field based YV12 VideoFrame with a non mod 4 height");        
        break;
      default:
        if (fieldBased && (height & 1))
          throw InternalError("Attempted to request a field based VideoFrame with an odd height");
    }
    return height;
  }
  static inline dimension HeightCheck(dimension height, bool fieldBased, const ColorSpace& space)
  {
    if (height <= 0)
      throw InternalError("Attempted to request a zero height VideoFrame");
    return HeightParityCheck(height, fieldBased, space);
  }

  //default constructor : no video, no audio
  VideoInfo();
  VideoInfo(const VideoInfo& other);


  /*
   * Video stuff
   */
private:

  const ColorSpace * pixel_type;  
  dimension width, height;    
  unsigned fps_numerator, fps_denominator;
  int frameCount;

  void CheckHasVideo() const {
    if (! HasVideo())
      throw InternalError("VideoInfo: Invalid State: no Video");
  }

public:


  bool HasVideo() const { return *pixel_type != NO_VIDEO; }

  bool IsColorSpace(const ColorSpace& space) const { return *pixel_type == space; } 
  bool HasProperty(ColorSpace::Property prop) const { return pixel_type->HasFlag(prop); }

  bool IsPlanar() const { return HasProperty(ColorSpace::PLANAR); }
 
  bool IsRGB() const { return HasProperty(ColorSpace::BGR); }      
  bool IsRGB24() const { return IsColorSpace(CS_BGR24); } 
  bool IsRGB32() const { return IsColorSpace(CS_BGR32); }

  bool IsYUV() const { return HasProperty(ColorSpace::YUV); }
  bool IsYUY2() const { return IsColorSpace(CS_YUY2); }  
  bool IsYV12() const { return IsColorSpace(CS_YV12) || IsColorSpace(CS_I420); }

  bool IsVPlaneFirst() const { return IsYV12(); }  // Don't use this 


  dimension GetWidth() const { CheckHasVideo(); return width; }
  dimension GetHeight() const { CheckHasVideo(); return height; }
  int GetFrameCount() const { CheckHasVideo(); return frameCount; }

  unsigned GetFPSNumerator() const { CheckHasVideo(); return fps_numerator; }
  unsigned GetFPSDenominator() const { CheckHasVideo(); return fps_denominator; }
  double GetFPS() const { CheckHasVideo(); return fps_numerator/fps_denominator; }

  const ColorSpace& GetColorSpace() const { return *pixel_type; }

  VideoInfo& RemoveVideo() { pixel_type = &NO_VIDEO; return *this; }
  VideoInfo  RemoveVideo() const { return VideoInfo(*this).RemoveVideo(); }

  VideoInfo& SetColorSpace(const ColorSpace& space) { WidthCheck(width, space); HeightCheck(height, IsFieldBased(), space); pixel_type = &space; return *this; }
  VideoInfo  SetColorSpace(const ColorSpace& space) const { return VideoInfo(*this).SetColorSpace(space); }

  VideoInfo& SetWidth(dimension _width) { CheckHasVideo(); width = WidthCheck(_width, *pixel_type); return *this; }
  VideoInfo  SetWidth(dimension _width) const { return VideoInfo(*this).SetWidth(_width); }
  VideoInfo& SetHeight(dimension _height) { CheckHasVideo(); height = HeightCheck(_height, IsFieldBased(), *pixel_type); return *this; }
  VideoInfo  SetHeight(dimension _height) const { return VideoInfo(*this).SetHeight(_height); }

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


  int BytesFromPixels(int pixels) const { return pixels * (BitsPerPixel()>>3); }   // Will not work on planar images, but will return only luma planes
  int RowSize() const { return BytesFromPixels(width); }  // Also only returns first plane on planar images
  int BMPSize() const { if (IsPlanar()) {int p = height * ((RowSize()+3) & ~3); p+=p>>1; return p;  } return height * ((RowSize()+3) & ~3); }

  int BitsPerPixel() const {
    CheckHasVideo();
    static int bits[] = { 0, 24, 32, 16, 12, 12 };
    return bits[pixel_type->id];
  }


  /*
   * Audio stuff
   */
private:

  SampleType sample_type;           
  int audio_samples_per_second;   // 0 means no audio
  __int64 num_audio_samples;      // changed as of 2.5
  int nchannels;                  // as of 2.5

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


 
  /*
   * Field based stuff
   */
private:

  int image_type;

  enum {
    IT_BFF = 1<<0,
    IT_TFF = 1<<1,
    IT_FIELDBASED = 1<<2
  };

public:
  bool IsFieldBased() const { return image_type & IT_FIELDBASED != 0; }
  bool IsBFF() const { return image_type & IT_BFF != 0; }
  bool IsTFF() const { return image_type & IT_TFF != 0; }
  bool IsParityKnown() const { return IsFieldBased() && (IsBFF() || IsTFF()); }
  
  void SetFieldBased(bool isfieldbased)  { 
    if (isfieldbased) {
      HeightCheck(height, true, *pixel_type);
      image_type |= IT_FIELDBASED;
    } else 
        image_type &= ~IT_FIELDBASED;
  }
  void SetBFF() { image_type |= (IT_FIELDBASED | IT_BFF); image_type &= ~IT_TFF; }
  void SetTFF() { image_type |= (IT_FIELDBASED | IT_TFF); image_type &= ~IT_BFF; }

};



#endif  //#ifndef __VIDEOINFO_H__