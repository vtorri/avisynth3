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


#ifndef __AVS_FILTERS_SOURCE_VIDEO_FRAMEMAKER_H__
#define __AVS_FILTERS_SOURCE_VIDEO_FRAMEMAKER_H__

//avisynth includes
#include "forward.h"                           //for PImporter
#include "../../../core/forward.h"             //for Dimension typedef
#include "../../../core/framemaker.h"
#include "../../../core/ownedblock.h"
#include "../../../core/geometry/dimension.h"  //so Dimension is defined

//stl include
#include <memory>                              //so PImporter is defined


namespace avs { namespace filters { namespace source { namespace video {



////////////////////////////////////////////////////////////////////////////////////////
//  source::video::FrameMaker
//
//
//
class AVS_NOVTABLE FrameMaker : public avs::FrameMaker
{

  Dimension dim_;
  PImporter importer_;


public:  //structors

  FrameMaker(Dimension const& dim)
    : dim_( dim ) { }

  FrameMaker(Dimension const& dim, PImporter const& importer)
    : dim_( dim )
    , importer_( importer ) { }

  //generated destructor is fine


public:  //FrameMaker interface

  virtual CPVideoFrame MakeFrame(long n) const;


public:  //video::FrameMaker interface

  virtual owned_block<1> GetRawFrame(long n) const = 0;


protected:  //for subclass constructor use

  Dimension const& GetDimension() const { return dim_; }
  void SetImporter(PImporter const& importer) { importer_ = importer; }

};



} } } } //namespace avs::filters::source::video

#endif //__AVS_FILTERS_SOURCE_VIDEO_FRAMEMAKER_H__
