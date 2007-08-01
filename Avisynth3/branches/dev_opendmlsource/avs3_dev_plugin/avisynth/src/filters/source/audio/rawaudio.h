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


#ifndef __AVS_FILTERS_SOURCE_RAWAUDIO_H__
#define __AVS_FILTERS_SOURCE_RAWAUDIO_H__

//avisynth includes
#include "audiodecompressor.h"
#include "../../../clip/nochild.h"

//boost include
#include <boost/scoped_ptr.hpp>


namespace avs { namespace filters { namespace source {



class RawAudio : public virtual clip::NoChild
{

  boost::scoped_ptr<AudioDecompressor const> audioDecompressor_;


public:  //Clip general interface

  virtual BYTE * GetAudio(BYTE * buffer, long long start, long count) const;


public:  //RawAudio interface

  virtual long ReadAudio(BYTE * buffer, long long start, long& size) const = 0;


protected:  //write access

  void SetAudioDecompressor(AudioDecompressor * audioDecompressor) { audioDecompressor_.reset(audioDecompressor); }

};



} } } //namespace avs::filters::source

#endif //__AVS_FILTERS_SOURCE_RAWAUDIO_H__
