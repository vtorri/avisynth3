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
#include "vbracmaudiodecompressor.h"
#include "../../../vfw/waveformatex.h"


namespace avs { namespace filters { namespace source {



VBRACMAudioDecompressor::VBRACMAudioDecompressor(RawAudio const& src, vfw::WaveFormatEx const& input, vfw::WaveFormatEx const& output)
  : ACMAudioDecompressor(src, input, output)
  , step_( output.nSamplesPerSec * output.nBlockAlign )
{
  index_[0LL] = 0LL;
}


void VBRACMAudioDecompressor::Seek(long long current) const
{
  //get the smallest of the strictly greater than current
  PositionToBlockMap::const_iterator it = index_.upper_bound(current);
  --it;        //change it to the biggest of the smaller than current (exists: 0 is in the map)

  SetPosition(it->second, it->first);
}


void VBRACMAudioDecompressor::Register(long long block, long long current) const
{
  //get the smallest of the strictly greater than current
  PositionToBlockMap::iterator it = index_.upper_bound(current);

  if ( it != index_.end() )                   //if not at end
    if ( it->first - current < step_ )        //if gap too small
      return;                                 //exits without registering position

  --it;        //change it to the biggest of the smaller than current (exists: 0 is in the map)

  if ( step_ <= current - it->first )         //if gap big enough
    index_[current] = block;                  //registers position

}


} } } //namespace avs::filters::source

#endif //_WIN32
