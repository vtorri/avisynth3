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


#ifndef __AVS_FREETYPE_BITMAPHOLDER_H__
#define __AVS_FREETYPE_BITMAPHOLDER_H__

//avisynth includes
#include "../core/block/holder.h"  
#include "../core/block/holder/standard.h"

//boost include
#include <boost/shared_ptr.hpp>

//freetype declaration
struct FT_BitmapGlyphRec_;


namespace avs { namespace freetype {



class BitmapHolder : public block::Holder
{

  boost::shared_ptr<FT_BitmapGlyphRec_> bitmapGlyph_;


public:  //structors

  BitmapHolder(boost::shared_ptr<FT_BitmapGlyphRec_> const& bitmapGlyph)
    : bitmapGlyph_( bitmapGlyph ) { }

  //generated destructor is fine


public:  //Holder interface

  virtual int size() const;
  virtual BYTE * get() const;

  virtual Holder * spawn(int size, bool recycle) const { return new block::holder::Standard(size, false); }

};


} } //namespace avs::freetype

#endif //__AVS_FREETYPE_BITMAPHOLDER_H__
