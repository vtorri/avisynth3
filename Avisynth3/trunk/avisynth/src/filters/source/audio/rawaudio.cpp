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


//avisynth includes
#include "rawaudio.h"
#include "../../../core/videoinfo.h"
#include "../../../core/exception/noaudio.h"


namespace avs { namespace filters { namespace source {



BYTE * RawAudio::GetAudio(BYTE * buffer, long long start, long count) const
{
  if ( ! audioDecompressor_ )                   //ie if clip has no audio
    throw exception::NoAudio();                 //then no audio

  CPVideoInfo vi = GetVideoInfo();              //fetchs video info

  if ( start < 0 )                              //if requests samples before clip
    if ( count < -start )                       //if all samples are before
      return vi->GetBlankNoise(buffer, count);  //get video info to fill the whole with blank
    else 
    {                                           //fill -start blank samples
      buffer = vi->GetBlankNoise(buffer, static_cast<long>(-start));
      count += static_cast<long>(start);        //-start less samples to provide
      start = 0;                                //now continue at 0
    }

  //use decompressor to get samples, returns obtained sample count
  long filled = audioDecompressor_->operator()(buffer, start, count);

  if ( filled < count )                         //if we got less that we wanted
    buffer = vi->GetBlankNoise(buffer, count - filled);    //fill with blank

  return buffer;
}



} } } //namespace avs::filters::source
