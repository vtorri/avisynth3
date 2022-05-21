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


#ifndef __AVS_FILTERS_SOURCE_VBRACMAUDIODECOMPRESSOR_H__
#define __AVS_FILTERS_SOURCE_VBRACMAUDIODECOMPRESSOR_H__

//avisynth include
#include "acmaudiodecompressor.h"

//stl include
#include <map>


namespace avs { namespace filters { namespace source {



class VBRACMAudioDecompressor : public ACMAudioDecompressor
{

  typedef std::map<long long, long long> PositionToBlockMap;

  mutable PositionToBlockMap index_;
  long step_;                            //minimal gap between entries in the above index


public:  //structors

  VBRACMAudioDecompressor(RawAudio const& src, vfw::WaveFormatEx const& input, vfw::WaveFormatEx const& output);
//  : ACMAudioDecompressor(src, input, output)
//  , step_( output.nSamplesPerSec * output.nBlockAlign ) { }

  //generated destructor is fine


private:  //ACMAudioDecompressor interface

  virtual void Seek(long long current) const;

  virtual void Register(long long block, long long current) const;

};



} } } //namespace avs::filters::source

#endif //__AVS_FILTERS_SOURCE_VBRACMAUDIODECOMPRESSOR_H__
