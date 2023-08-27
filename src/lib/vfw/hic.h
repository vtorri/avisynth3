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


#ifndef __AVS_VFW_HIC_H__
#define __AVS_VFW_HIC_H__

//avisynth include
#include "bitmapinfoheader.h"

//stl include
#include <memory>

//windows includes
#ifndef NOMINMAX
#define NOMINMAX          //prevents generation of min and max macros
#endif //NOMINMAX
#include <windows.h>
#include <vfw.h>


namespace avs { namespace vfw {



//////////////////////////////////////////////////////////////////////////////////////////
//  vfw::Hic
//
//  smart handle replacement for VFW HIC
//
class Hic
{

  std::shared_ptr<HIC__> hic_;    //underlying VFW handle
  bool ex_;                       //flag marking whether we use -Ex methods or not


public:  //structors

  Hic(DWORD fourcc);
  Hic(BitmapInfoHeader const& bih);

  //generated copy constructor and destructor are fine


public:  //interface

  bool DecompressQuery(BitmapInfoHeader const& input, BitmapInfoHeader const& output) const;

  void DecompressBegin(BitmapInfoHeader const& input, BitmapInfoHeader const& output);
  void DecompressEnd();

  void Decompress(BitmapInfoHeader const& input, BitmapInfoHeader const& output, BYTE const * src, BYTE * dst, bool keyframe, bool preroll) const;

};


} } //namespace avs::vfw

#endif //__AVS_VFW_HIC_H__
