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

#ifndef __AVS_FILTERS_SOURCE_ACMAUDIODECOMPRESSOR_H__
#define __AVS_FILTERS_SOURCE_ACMAUDIODECOMPRESSOR_H__

//avisynth includes
#include "rawaudio.h"
#include "audiodecompressor.h"
#include "../../../vfw/forward.h"   //for PWaveFormatEx

//boost includes
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

//windows includes
#define NOMINMAX          //prevents generation of min and max macros
#include <windows.h>
#include <Mmreg.h>
#include <Msacm.h>


namespace avs { namespace filters { namespace source {



class ACMAudioDecompressor : public AudioDecompressor
{
 
  RawAudio const& src_;

  boost::shared_ptr<HACMSTREAM__> hACMStream_;  //underlying handle to acm conversion stream
  boost::scoped_ptr<BYTE> inputBuffer_;         //input buffer
  boost::scoped_ptr<BYTE> outputBuffer_;        //output buffer
	mutable ACMSTREAMHEADER ash_;                 //struct used to pass data to acm

	mutable long long current_;                   //current reading position in bytes
  mutable long long nextBlock_;                 //next stream block to decompress from

  long blockAlign_;
  long samplesPerSec_;
  long avgBytesPerSec_;

	enum { inputBufferSize = 16384 };


public:  //structors

  ACMAudioDecompressor(RawAudio const& src, vfw::PWaveFormatEx& wfe);

  virtual ~ACMAudioDecompressor();


public:  //AudioDecompressor interface

  virtual long operator()(BYTE *& buffer, long long start, long count) const;


private:  //implementation

  void DecompressNext() const;

  long Read(BYTE *& buffer, long size) const;
  long ReadFromBuffer(BYTE *& buffer, long& size) const;

  void Skip(long size) const;
  long SkipFromBuffer(long size) const;


public:  //factory method

  static AudioDecompressor * Create(RawAudio const& src, vfw::PWaveFormatEx& wfe);

};



} } } //namespace avs::filters::source

#endif //__AVS_FILTERS_SOURCE_ACMAUDIODECOMPRESSOR_H__

#endif //_WIN32
