// Avisynth v3.0 alpha.  Copyright 2004 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_VIDEOINFO_CONCRETE_H__
#define __AVS_VIDEOINFO_CONCRETE_H__


//avisynth includes
#include "../videoinfo.h"
#include "../colorspace.h"
#include "../exception/generic.h"
#include "../geometry/dimension.h"

//boost include
#include <boost/optional.hpp>


namespace avs { namespace videoinfo {



class Concrete : public VideoInfo
{

  struct VideoProperties
  {

    ColorSpace * space_;
    Dimension dim_;
    int frameCount_;
    Fraction fps_;
    bool frameClip_;

    VideoProperties(ColorSpace& space, Dimension const& dim, int frameCount, Fraction const fps, bool frameClip)
      : space_( &space )
      , dim_( dim )
      , frameCount_( frameCount )
      , fps_( fps )
      , frameClip_( frameClip ) { }

  };//VideoProperties


  struct AudioProperties
  {

    SampleType sampleType_;
    int sampleRate_;
    long long sampleCount_;
    int channelCount_;

    AudioProperties(SampleType sampleType, int sampleRate, long long sampleCount, int channelCount)
      : sampleType_( sampleType )
      , sampleRate_( sampleRate )
      , sampleCount_( sampleCount )
      , channelCount_( channelCount ) { }

  };//AudioProperties


private:  //members

  boost::optional<VideoProperties> video_;
  boost::optional<AudioProperties> audio_;


public:  //structors

  //generated ones are fine


public:  //clone method

  virtual CPVideoInfo clone() const { return CPVideoInfo( (VideoInfo *)new Concrete(*this) ); }


public:  //video methods

  virtual bool HasVideo() const { return video_; }

  virtual ColorSpace& GetColorSpace() const { CheckHasVideo(); return *video_->space_; }
  virtual Dimension const& GetDimension() const { CheckHasVideo(); return video_->dim_; }
  virtual int GetFrameCount() const { CheckHasVideo(); return video_->frameCount_; }
  virtual Fraction const& GetFPS() const { CheckHasVideo(); return video_->fps_; }
  virtual bool IsFrameClip() const { CheckHasVideo(); return video_->frameClip_; }

  //write access

  virtual void SetColorSpace(ColorSpace& space) { CheckHasVideo(); space.CheckDim(video_->dim_); video_->space_ = &space; }
  virtual void SetDimension(Dimension const& dim) { CheckHasVideo(); video_->space_->CheckDim(dim); video_->dim_ = dim; }
  virtual void SetFrameCount(int frameCount) { CheckHasVideo(); video_->frameCount_ = CheckFrameCount(frameCount); }
  virtual void SetFPS(Fraction const& fps) { CheckHasVideo(); video_->fps_ = fps; }
  virtual void SetIsFrameClip(bool frameClip) { CheckHasVideo(); video_->frameClip_ = frameClip; }


  virtual void AddVideo(ColorSpace& space, Dimension const& dim, int frameCount, Fraction const& fps = 25, bool frameClip = true)
  {
    space.CheckDim(dim);
    video_ = VideoProperties(space, dim, CheckFrameCount(frameCount), fps, frameClip);
  }

  virtual void KillVideo() { video_ = boost::detail::none_t(); }


public:  //audio methods

  virtual bool HasAudio() const { return audio_; }

  virtual SampleType GetSampleType() const { CheckHasAudio(); return audio_->sampleType_; }
  virtual int GetSampleRate() const { CheckHasAudio(); return audio_->sampleRate_; }
  virtual long long GetSampleCount() const { CheckHasAudio(); return audio_->sampleCount_; }
  virtual int GetChannelCount() const { CheckHasAudio(); return audio_->channelCount_; }

  //write access

  virtual void SetSampleType(SampleType sampleType) { CheckHasAudio(); audio_->sampleType_ = CheckSampleType(sampleType); }
  virtual void SetSampleRate(int sampleRate) { CheckHasAudio(); audio_->sampleRate_ = CheckSampleRate(sampleRate); }
  virtual void SetSampleCount(long long sampleCount) { CheckHasAudio(); audio_->sampleCount_ = CheckSampleCount(sampleCount); }
  virtual void SetChannelCount(int channelCount) { CheckHasAudio(); audio_->channelCount_ = CheckChannelCount(channelCount); }


  virtual void AddAudio(SampleType sampleType, int sampleRate, long long sampleCount, int channelCount)
  {
    audio_ = AudioProperties(CheckSampleType(sampleType), CheckSampleRate(sampleRate), CheckSampleCount(sampleCount), CheckChannelCount(channelCount));
  }

  virtual void KillAudio() { audio_ = boost::detail::none_t(); }

  virtual void MergeAudio(VideoInfo const& other) { audio_ = static_cast<Concrete const&>(other).audio_; }


private:

  static int CheckFrameCount(int frameCount) 
  { 
    if ( frameCount < 0 )
      throw exception::Generic("FrameCount must be positive");
    return frameCount; 
  }
  static SampleType CheckSampleType(SampleType sampleType) { return sampleType; }
  static int CheckSampleRate(int sampleRate) { return sampleRate; }
  static long long CheckSampleCount(long long sampleCount) { return sampleCount; }
  static int CheckChannelCount(int channelCount) { return channelCount; }

};


} } //namespace avs

#endif //__AVS_VIDEOINFO_CONCRETE_H__
