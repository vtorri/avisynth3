// Avisynth v3.0 alpha.  Copyright 2004 Ben Rudiak-Gould et al.
// http://www.avisynth.org
// This file is Copyleft kurosu@inforezo.org

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

#ifndef __AVS_FILTERS_FFMPEG_H__
#define __AVS_FILTERS_FFMPEG_H__

#define FF_DEBUG             1

#if defined(_WIN32) && FF_DEBUG
#  include <windows.h>
#  include <iostream>
#endif

//avisynth include
#include "../../clip/nochild.h"
#include "../../clip/caching/concrete.h"
#include "../../clip/caching.h"
#include "../../core/forward.h"

//stl include
#include <sstream>
#include <string>
#include <deque>

namespace avs { namespace filters {

#include <avformat.h>

#if FF_DEBUG
#  ifdef _WIN32
#    define DEBUG_LOG2(x)				 \
    stroffset = debug_buffer.tellp();			 \
    debug_buffer << x
#    define DEBUG_LOG(x)				 \
    DEBUG_LOG2(x << "\n");				 \
    OutputDebugString(debug_buffer.str().c_str()+\
		      stroffset);			 \
    debug_buffer.clear();				 \
    debug_buffer.seekp(0, std::ios_base::beg);

#  else
#    define DEBUG_LOG(x) std::cout << x << std::endl;
#    define DEBUG_LOG2(x) std::cout << x;
#  endif
#else
#  define DEBUG_LOG(x) (void)0
#  define DEBUG_LOG2(x) (void)0
#endif

  typedef boost::shared_ptr<AVPacket> PAVPacket;
  typedef std::deque<PAVPacket> PacketQueue;
  struct PacketDestructor
  {
    void operator()(AVPacket * pkt) const
    {
      if (pkt == NULL) return;
      av_free_packet(pkt);
      av_free(pkt);
    }
  };
    

  //NB: A PTS is a Presentation TimeStamp; this is ffmpeg's usual
  //    unit rather than frames or samples
  class FFmpegSource : public clip::NoChild, clip::caching::Concrete
  {
  private:
    CPVideoInfo vi_;

    //Debug - let's do it simply
#  if FF_DEBUG
#    ifdef _WIN32
    mutable std::ostringstream debug_buffer;
    mutable int stroffset;
#    endif
#  endif

    // Format
    void OpenFileAndFindStreams(const char *name);
    void GetVideoStreamInfo(PVideoInfo &vi, std::string const& csname);
    void GetAudioStreamInfo(PVideoInfo &vi);
    AVInputFormat *fmt;
    AVFormatContext *ic;
    AVFormatParameters ap;

    //Colorspace fiddling
    ColorSpace& CreateColorSpace(std::string const& csname, PixelFormat pix_fmt);
    ColorSpace& MatchColorSpace(PixelFormat pix_fmt);
    PixelFormat conversion_pix_fmt;
    AVFrame *conversion_avf;
    unsigned char *conversion_buf;

    //Blitting
    void ColorSpaceAwareBlit(PVideoFrame &frame) const;
    void PlaneAwareBlit(PVideoFrame &frame, AVFrame *blit_avf) const;

    // Codecs
    AVCodec *audio, *video;
    AVCodecContext *audio_ctx, *video_ctx;
    int audioIndex_, videoIndex_;

    // Seeking related
    long long GetPTSFromFrame(int n) const;
    long long GetPTSFromSample(long long start) const;
    void FlushStreams() const;
    // Read a packet of specified stream; throws:
    // -1 -> Error
    //  0 -> OK
    //  1 -> Special packet
    PAVPacket ReadFrame(long long int pts, int index) const;
    mutable int numpkt;
    mutable PacketQueue videoQueue_, audioQueue_;
    mutable PAVPacket pendingVideoPacket_, pendingAudioPacket_;
    mutable long long int last_pts;
    mutable int videoBytesRemaining_, audioBytesRemaining_;
    mutable int last_frame;

    // Video decoding
    AVFrame *avf;
    mutable unsigned char *rawData;
    mutable int pict_type;
    long long GetNextFrame(long long pts) const;

public:  //constructor
    FFmpegSource(std::string const& name, std::string const& csname,
		 PEnvironment const& env);
    ~FFmpegSource();

public:  //clip general interface
    virtual CPVideoInfo GetVideoInfo() const { return vi_; }
    virtual CPVideoFrame MakeFrame(int n) const;
    virtual void GetAudio(void* buffer, long long start, int count) const;

public:  //factory methods
    static PClip Create(std::string const& name, std::string const& csname,
			PEnvironment const& env)
    {
      return PClip( static_cast<Clip *>(new FFmpegSource(name, csname, env)) );
    }

public: //My own methods, if ever needed for post-processing
    int GetLastPictureType()
    {
      if (avf) return avf->pict_type;
      else return 0;
    };
    int GetLastPictureQPTableStride()
    {
      if (avf) return avf->qstride;
      else return 0;
    }
    const int8_t *GetLastPictureQPTable()
    {
      if (avf) return avf->qscale_table;
      else return NULL;
    }
      
  };

} } //namespace avs::filters
#endif //__AVS_FILTERS_FFMPEGSOURCE_H__


