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


#ifndef _WIN32

//avisynth includes
#include "audio.h"
#include "../../core/videoinfo.h"
#include "../../core/exception/generic.h"

//stl include
#include <string>


namespace avs { namespace gstreamer { namespace structure {



void Audio:SetVideoInfo(VideoInfo& vi) const
{
  vi.AddAudio(GetSampleType(), GetIntField("rate"), 0, GetIntField("channels"));
}


SampleType Audio::GetSampleType() const
{
  std::string name = GetName();

  if ( name == "audio/x-raw-int" )
  {
    int width = GetIntField("width");

    if ( width == GetIntField("depth") )    
      if ( ! GetBoolField("signed") )    //if unsigned
        if ( width == 8 )                //and width 8
          return SAMPLE_INT8;
        else                             //signed
          switch ( width )
          {
          case 16: return SAMPLE_INT16;
          case 24: return SAMPLE_INT24;
          case 32: return SAMPLE_INT32;
          default: break;
          }
  }

  if ( name == "audio/x-raw-float" && GetIntField("width") == 32 )
    return SAMPLE_FLOAT;

  throw exception::Generic("no matching sample type");
}



} } } //namespace avs::gstreamer::structure

#endif //_WIN32
