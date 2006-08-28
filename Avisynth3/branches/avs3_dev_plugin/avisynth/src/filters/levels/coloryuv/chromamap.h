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


#ifndef __AVS_FILTERS_COLORYUV_CHROMAMAP_H__
#define __AVS_FILTERS_COLORYUV_CHROMAMAP_H__

//avisynth includes
#include "levels.h"
#include "../../../utility/bytemap.h"


namespace avs { namespace filters { namespace coloryuv {



/////////////////////////////////////////////////////////////////////////////////////
//  coloryuv::ChromaMap
//
//  look up table used by ColorYUV on the chroma
//
class ChromaMap : public ByteMap
{

public:  //structors

  ChromaMap(Levels const& levels, int (* adjust)(int), bool coring);

  //generated copy constructor and destructor are fine


public:  //assignement

  ChromaMap& operator=(ChromaMap const& other) { return static_cast<ChromaMap&>( ByteMap::operator=(other) ); }

  void swap(ChromaMap& other) { ByteMap::swap(other); }


public:  //methods usable as parameter in the constructor

  enum { shift = 1 << 10 };

  template <int pcFactor, int tvFactor>
  static int PCtoTVAdjust(int value)
  {
    return ( (value - 128) * shift * tvFactor / pcFactor + 128 * shift + shift / 2 ) / shift;
  }

  template <int pcFactor, int tvFactor>
  static int TVtoPCAdjust(int value)
  {
    return ( value * shift * pcFactor / tvFactor + 16 * shift + shift / 2 ) / shift;
  }

};




} } } //namespace avs::filters::coloryuv

#endif //__AVS_FILTERS_COLORYUV_CHROMAMAP_H__
