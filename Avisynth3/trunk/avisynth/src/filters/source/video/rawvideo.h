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


#ifndef __AVS_FILTERS_SOURCE_RAWVIDEO_H__
#define __AVS_FILTERS_SOURCE_RAWVIDEO_H__

//avisynth includes
#include "framedecompressor.h"
#include "../../../clip/caching.h"
#include "../../../clip/nochild.h"

//boost include
#include <boost/scoped_ptr.hpp>


namespace avs { namespace filters { namespace source {



class RawVideo : public virtual clip::NoChild
               , public virtual clip::Caching
{

  boost::scoped_ptr<FrameDecompressor const> frameDecompressor_;

  mutable long lastIndex_;


public:  //MakeFrame method

  virtual CPVideoFrame MakeFrame(long n) const;


public:  //RawVideo interface

  //returns index of previous keyframe
  virtual long PreviousKeyFrame(long n) const = 0;

  //reads video data for frame n into passed buffer and returns read data size
  //can be passed a NULL buffer to get the necessary buffer size
  virtual long ReadVideo(long n, BYTE * buffer, long bufferSize) const = 0;


protected:  //write access

  void SetFrameDecompressor(FrameDecompressor * frameDecompressor) { frameDecompressor_.reset(frameDecompressor); }

};



} } } //namespace avs::filters::source

#endif //__AVS_FILTERS_SOURCE_RAWVIDEO_H__