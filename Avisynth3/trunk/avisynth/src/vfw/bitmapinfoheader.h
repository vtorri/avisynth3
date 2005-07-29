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


#ifndef __AVS_VFW_BITMAPINFOHEADER_H__
#define __AVS_VFW_BITMAPINFOHEADER_H__

//avisynth includes
#include "forward.h"                           //for PExporter, PImporter typedefs
#include "../core/forward.h"                   //for VideoInfo, Dimension, PImporter
#include "../core/geometry/dimension.h"        //so Dimension is defined

//boost include
#include <boost/shared_ptr.hpp>                //so Pimporter is defined

//windows includes
#ifndef NOMINMAX
#define NOMINMAX          //prevents generation of min and max macros
#endif //NOMINMAX
#include <windows.h>
#include <wingdi.h>


namespace avs { namespace vfw {



/////////////////////////////////////////////////////////////////////////////////
//  BitmapInfoHeader
//
//  a more friendly replacement for VFW BITMAPINFOHEADER
//
class BitmapInfoHeader : public BITMAPINFOHEADER
{

public:  //structors

  BitmapInfoHeader(Dimension const& dim) { Init(dim); }
  BitmapInfoHeader(Dimension const& dim, Exporter const& exporter) { Init(dim); SetExporter(exporter); }

  //generated copy constructor and destructor are fine


public:  //read access

  Dimension GetDimension() const { return Dimension(biWidth, biHeight); }
  
  //returns empty if cannot be done
  PImporter GetImporter() const;


public:  //write access

  void SetExporter(Exporter const& exporter);


/*public:  //comparison operator

  bool operator==(BitmapInfoHeader const& other) const;*/


private:  //implementation helper

  void Init(Dimension const& dim);

};



} } //namespace avs::vfw

#endif //__AVS_VFW_BITMAPINFOHEADER_H__
