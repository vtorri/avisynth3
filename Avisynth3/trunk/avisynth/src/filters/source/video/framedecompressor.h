// Avisynth v3.0 alpha.  Copyright 2005 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_FILTERS_SOURCE_FRAMEDECOMPRESSOR_H__
#define __AVS_FILTERS_SOURCE_FRAMEDECOMPRESSOR_H__

//avisynth includes
#include "../../../define.h"              //for AVS_NOVTABLE
#include "../../../core/ownedblock.h"
#include "../../../core/bufferwindow.h"


namespace avs { namespace filters { namespace source {


//forward declaration
class RawVideo;
  
  

/////////////////////////////////////////////////////////////////////////////////////
//  FrameDecompressor
//
//  takes care of decompressing the input video data to a format readable by avs
//
class AVS_NOVTABLE FrameDecompressor
{

  RawVideo const& src_;      //back reference to owner


public:  //structors

  FrameDecompressor(RawVideo const& src)
    : src_( src ) { }

  virtual ~FrameDecompressor() { }


public:  //FrameDecompressor public interface

  CPVideoFrame operator()(long n, bool preroll) const;

  enum { Guard = BufferWindow::Guard };


private:  //FrameDecompressor private interface

  //NB: in both passed and returned block, the data starts at + BufferWindow::Guard (for later promotion)
  virtual OwnedBlock operator()(bool keyframe, bool preroll, OwnedBlock const& block, long bytesRead) const = 0;

};


} } } //namespace avs::filters::source

#endif //__AVS_FILTERS_SOURCE_FRAMEDECOMPRESSOR_H__
