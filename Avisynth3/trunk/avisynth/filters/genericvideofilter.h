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


#ifndef __AVS_GENERICVIDEOFILTER_H__
#define __AVS_GENERICVIDEOFILTER_H__


//avisynth includes
#include "videoinfoclip.h"
#include "stablevideofilter.h"


#pragma warning ( push )           //push warning state
#pragma warning (disable : 4250)   //get rid of MI dominance decisions



///////////////////////////////////////////////////////////////////////
// GenericVideoFilter
//
// general case: a child clip, cache needed, videoinfo altered
// like the above, subclass should override MakeFrame
//
class GenericVideoFilter : public StableVideoFilter, public VideoInfoClip
{

public:  //constructors

  //norma constructor, does not set up VideoInfo
  GenericVideoFilter(PClip child, const CachePolicy& policy = DefaultCachePolicy())
    : StableVideoFilter( child, policy ) { }

  //copy constructor
  GenericVideoFilter(const GenericVideoFilter& other)
    : StableVideoFilter( other )
    , VideoInfoClip( other ) { }


public:  //clip public interface

  //resolving MI ambiguity
  virtual const VideoInfo& GetVideoInfo() const { return VideoInfoClip::GetVideoInfo(); }

};





#pragma warning ( pop )


#endif //__AVS_GENERICVIDEOFILTER_H__