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


#ifdef _WIN32

//avisynth includes
#include "rawvideo.h"
#include "../avifilesource.h"                 //for AviFileSource::ReadFormat
#include "../../../core/videoinfo.h"
#include "../../../vfw/avistreaminfo.h"
#include "../../../vfw/bitmapinfoheader.h"
#include "../video/icmframedecompressor.h"
#include "../../../core/exception/generic.h"

//windows includes
#ifndef NOMINMAX
#define NOMINMAX
#endif //NOMINMAX
#include <windows.h>
#include <vfw.h>


namespace avs { namespace filters { namespace source { namespace avifile {




long RawVideo::PreviousKeyFrame(long n) const
{
  return video_->FindSample(n, FIND_KEY | FIND_PREV);
}


long RawVideo::ReadVideo(long n, BYTE * buffer, long bufferSize) const
{
  long size = 0;

  if ( video_->Read(n, 1, buffer, bufferSize, &size, NULL) != AVIERR_OK )
    throw exception::Generic("Cannot read from source file");

  return size;
}


void RawVideo::InitVideo(PAVIStream const& video, VideoInfo& vi)
{
  if ( video )
  {
    video_ = video;

    //fetch format header from stream
    vfw::PBitmapInfoHeader bih = boost::static_pointer_cast<vfw::BitmapInfoHeader>(AviFileSource::ReadFormat(video));

    PColorSpace space;
    //sets frame decompressor (and gets output colorspace)
    SetFrameDecompressor( ICMFrameDecompressor::Create(*this, bih, space) );
    vfw::AviStreamInfo asi(*video);

    vi.AddVideo(space, bih->GetDimension(), 0);
    asi.SetLengthsTo(vi);
  }
}



} } } } //namespace avs::filters::source::avifile

#endif //_WIN32
