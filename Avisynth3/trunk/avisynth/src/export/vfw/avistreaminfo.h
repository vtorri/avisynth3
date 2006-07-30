// Avisynth v3.0 alpha.  Copyright 2003-2006 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_EXPORT_VFW_AVISTREAMINFO_H__
#define __AVS_EXPORT_VFW_AVISTREAMINFO_H__

//avs includes
#include "forward.h"                          //for Exporter
#include "../../import/avi/streamheader.h"

struct IAVIStream;


namespace avs { namespace export_ { namespace vfw {



/////////////////////////////////////////////////////////////////////////////////////
//  AviStreamInfo
//
//  avs C++ version of the VFW struct AVISTREAMINFOW
//
//  Though they are not related through inheritance,
//  the layout is the same, so cast inbetween are safe
//
struct AviStreamInfo : public import::avi::StreamHeader
{

  unsigned long editCount;
  unsigned long formatChangeCount;
  wchar_t name[64];


public:  //structors

  AviStreamInfo();
  AviStreamInfo(IAVIStream& stream);


public:  //inner subclasses

  struct Video;
  struct Audio;

};


struct AviStreamInfo::Video : public AviStreamInfo
{
  Video(VideoInfo const& vi, Exporter const& exporter);
};

struct AviStreamInfo::Audio : public AviStreamInfo
{
  Audio(VideoInfo const& vi);
};



} } } //namespace avs::export_::vfw

#endif //__AVS_EXPORT_VFW_AVISTREAMINFO_H__
