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


#ifndef __AVS_FILTERS_SPLICE_AUDIOSWITCH_H__
#define __AVS_FILTERS_SPLICE_AUDIOSWITCH_H__

//avisynth include
#include "../splice.h"

//stl include
#include <vector>


namespace avs { namespace filters { namespace splice {



class AudioSwitch
{
  
  typedef std::vector<long long> SwitchVector;

  SwitchVector switchs_;


public:  //AudioSwitch interface

  BYTE * GetAudio(BYTE * buffer, long long start, long count, Splice const& splice) const;

  //these returns the total sample count, not clip contribution only
  long long Push(PClip const& clip, bool merging);
  long long PushAligned(PClip const& clip, bool merging);


private:  //implementation

  long long Push(long long count, bool merging);

};



} } } //namespace avs::filters::splice

#endif //__AVS_FILTERS_SPLICE_AUDIOSWITCH_H__
