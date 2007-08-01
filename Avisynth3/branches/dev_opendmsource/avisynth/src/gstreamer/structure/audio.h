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


#ifndef __AVS_GSTREAMER_STRUCTURE_AUDIO_H__
#define __AVS_GSTREAMER_STRUCTURE_AUDIO_H__

//avisynth includes
#include "../structure.h"
#include "../../core/forward.h"               //for VideoInfo declaration
#include "../../core/sampletype.h"


namespace avs { namespace gstreamer { namespace structure {



///////////////////////////////////////////////////////////////////////////////////////
//  structure::Audio
//
//
//
class Audio : public Structure
{

public:  //AudioStructure interface

  void SetVideoInfo(VideoInfo& vi) const;


private:  //helper

  SampleType GetSampleType() const;

};



} } } //namespace avs::gstreamer::structure

#endif //__AVS_GSTREAMER_STRUCTURE_AUDIO_H__
