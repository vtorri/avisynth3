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


#ifndef __AVS_FILTERS_SPLICE_STANDARD_H__
#define __AVS_FILTERS_SPLICE_STANDARD_H__

//avisynth includes
#include "../splice.h"
#include "audioswitch.h"
#include "videoswitch.h"


namespace avs { namespace filters { namespace splice {



////////////////////////////////////////////////////////////////////////////////////
//  splice::Standard
//
//  just a factorisation superclass for splice::Aligned and splice::Unaligned
//
class AVS_NOVTABLE Standard : public Splice
{

  AudioSwitch audio_;
  VideoSwitch video_;


public:  //structors

  Standard(CPVideoInfo const& vi, PEnvironment const& env)
    : Splice( vi, env ) { }

  //generated destructor is fine


public:  //clip general interface

  virtual CPVideoFrame GetFrame(int n) const { return video_.GetFrame(n, *this); }
  virtual BYTE * GetAudio(BYTE * buffer, long long start, int count) const { return audio_.GetAudio(buffer, start, count, *this); }


protected:  //Splice protected interface

  virtual void PushChild(PClip const& child);


private:  //Standard interface

  virtual long long PushAudio(AudioSwitch& audio, PClip const& child, bool merging) = 0;

};


} } } //namespace avs::filters::splice

#endif //__AVS_FILTERS_SPLICE_STANDARD_H__
