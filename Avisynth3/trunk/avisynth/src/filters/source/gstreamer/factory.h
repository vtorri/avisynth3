// Avisynth v3.0 alpha.  Copyright 2005 David Pierre - Ben Rudiak-Gould et al.
// http://www.avisynth.org
// This file is Copyleft Vincent Torri

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

#ifndef __AVS_FILTERS_SOURCE_GSTREAMER_FACTORY_H__
#define __AVS_FILTERS_SOURCE_GSTREAMER_FACTORY_H__

//avisynth includes
#include "forward.h"                           //for PPipeline typedef
#include "streamchooser.h"
#include "../../../core/forward.h"             //for PVideoInfo typedef
#include "../../../gstreamer/forward.h"        //for Pad declaration, PImporter typedef
#include "../../../core/cow_shared_ptr.h"      //so PVideoInfo is defined

//boost include
#include <boost/shared_ptr.hpp>                //so PImporter, PPipeline are defined

//stl include
#include <string>


namespace avs { namespace filters { namespace source { namespace gstreamer {



/////////////////////////////////////////////////////////////////////////////////////////
//  Factory
//
//
//
class Factory
{
  
  avs::gstreamer::PImporter importer_;
  PPipeline pipeline_;
  PVideoInfo vi_;

  StreamChooser videoChooser_;
  StreamChooser audioChooser_;


public:  //structors
  
  Factory(std::string const& name, int videoIndex, int audioIndex);
  
  //generated destructor is fine


public:  //access

  PVideoInfo const& vi() const { return vi_; }
  PPipeline const& pipeline() const { return pipeline_; }
  avs::gstreamer::PImporter const& importer() const { return importer_; }
  

public:  //used by callbacks
  
  void PadDetected(avs::gstreamer::Pad& pad);
  void Set(avs::gstreamer::structure::Video const& video);
  void Set(avs::gstreamer::structure::Audio const& audio);

};
  
  

} } } } // namespace avs::filters::source::gstreamer

#endif //__AVS_FILTERS_SOURCE_GSTREAMER_FACTORY_H__
