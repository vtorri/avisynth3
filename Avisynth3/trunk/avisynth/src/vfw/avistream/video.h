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


#ifndef __AVS_VFW_AVISTREAM_VIDEO_H__
#define __AVS_VFW_AVISTREAM_VIDEO_H__

//avisynth include
#include "../avistream.h"


namespace avs { 
  

//declarations
class Dimension;
class VideoFrame;

  
namespace vfw { namespace avistream {



class Video : public AviStream
{

  DWORD fccHandler_;


public:  //constructor

  Video(AviFile& parent, DWORD fccHandler)
    : AviStream(parent)
    , fccHandler_( fccHandler ) { }


public:  //IAVIStream

  STDMETHODIMP ReadFormat(LONG lPos, LPVOID lpFormat, LONG *lpcbFormat);
	STDMETHODIMP Read(LONG lStart, LONG lSamples, LPVOID lpBuffer, LONG cbBuffer, LONG *plBytes, LONG *plSamples);


protected:  //AviStream implementation

  virtual void FillAviStreamInfo(AVISTREAMINFOW& asi);
  virtual void Read(void* lpBuffer, int lStart, int lSamples);


protected:  //Video implementation

  virtual int GetBMPSize(Dimension const& dim) = 0;

  virtual void ReadFrame(VideoFrame const& frame, BYTE * ptr) = 0; 

};


} } } //namespace avs::vfw::avistream

#endif //__AVS_VFW_AVISTREAM_VIDEO_H__