// Avisynth v3.0 alpha.  Copyright 2003-2005 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_IMPORT_AVI_STREAMHEADER_H__
#define __AVS_IMPORT_AVI_STREAMHEADER_H__

//avisynth include
#include "../../core/forward.h"             //for VideoInfo declaration


namespace avs { namespace import { namespace avi {



////////////////////////////////////////////////////////////////////////////////
// StreamHeader
//
//
//
struct StreamHeader
{

  unsigned long	fccType;
  unsigned long fccHandler;
  unsigned long flags;
  unsigned short priority;
  unsigned short language;
  unsigned long	initialFrames;
  unsigned long scale;	
  unsigned long rate;	
  unsigned long	start;
  unsigned long	length;
  unsigned long	suggestedBufferSize;
  unsigned long	quality;
  unsigned long sampleSize;  
  
  struct Rectange
  {
    short left;
    short top;
    short right;
    short bottom;
  } frame;


public:

  void SetAsVideo(VideoInfo& vi) const;
  void SetAsAudio(VideoInfo& vi) const;


public:

  bool IsVideo() const { return fccType == 'SDIV'; }
  bool IsAudio() const { return fccType == 'SDUA'; }
  bool IsText() const { return fccType == 'STXT'; }

};



} } } //namespace avs::import::avi

#endif //__AVS_IMPORT_AVI_STREAMHEADER_H__
