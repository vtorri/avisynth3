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
#include "cbracmaudiodecompressor.h"
#include "../../../vfw/waveformatex.h"


namespace avs { namespace filters { namespace source {



CBRACMAudioDecompressor::CBRACMAudioDecompressor(RawAudio const& src, vfw::WaveFormatEx const& input, vfw::WaveFormatEx const& output)
  : ACMAudioDecompressor(src, input, output)
  , numerator_( input.nAvgBytesPerSec )
  , denominator_( static_cast<long long>(input.nBlockAlign) * input.nSamplesPerSec * output.nBlockAlign ) { }


void CBRACMAudioDecompressor::Seek(long long current) const
{
  //round to nearest audio block
  long long block = (current * numerator_) / denominator_;
  long long rounded = (block * denominator_) / numerator_;

  SetPosition(block, rounded);    //set self reading position

  Skip(current - rounded);        //skip to reach current
}



} } } //namespace avs::filters::source

#endif //_WIN32
