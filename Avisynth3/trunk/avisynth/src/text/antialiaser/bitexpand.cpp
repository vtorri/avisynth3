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


//avisynth include
#include "bitexpand.h"


namespace avs { namespace text { namespace aliaser {


BitExpandLeft::BitExpandLeft()
{
  for( int i = 0; i < 256; ++i )
  {
    int l = 0;
    if ( i &   1 ) l |= 0xFF;
    if ( i &   2 ) l |= 0xFE;
    if ( i &   4 ) l |= 0xFC;
    if ( i &   8 ) l |= 0xF8;
    if ( i &  16 ) l |= 0xF0;
    if ( i &  32 ) l |= 0xE0;
    if ( i &  64 ) l |= 0xC0;
    if ( i & 128 ) l |= 0x80;

    (*this)[i] = l;
  }
}


BitExpandRight::BitExpandRight()
{
  for( int i = 0; i < 256; ++i )
  {
    int r = 0;
    if ( i &   1 ) r |= 0x01;
    if ( i &   2 ) r |= 0x03;
    if ( i &   4 ) r |= 0x07;
    if ( i &   8 ) r |= 0x0F;
    if ( i &  16 ) r |= 0x1F;
    if ( i &  32 ) r |= 0x3F;
    if ( i &  64 ) r |= 0x7F;
    if ( i & 128 ) r |= 0xFF;

    (*this)[i] = r;
  }
}


BitExpandLeft const BitExpandLeft::instance = BitExpandLeft();

BitExpandRight const BitExpandRight::instance = BitExpandRight();


} } } //namespace avs::text::aliaser
