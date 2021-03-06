// Avisynth v3.0 alpha.  Copyright 2004 Ben Rudiak-Gould et al.
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


#ifndef __AVS_TEXT_ALIASER_BITEXPAND_H__
#define __AVS_TEXT_ALIASER_BITEXPAND_H__

//avisynth include
#include "../../utility/bytemap.h"


namespace avs { namespace text { namespace aliaser {



//////////////////////////////////////////////////////////////////////////////
//  BitExpandLeft
//
//  sets bit at 1 on the left of already set bits
//
class BitExpandLeft : public ByteMap
{

private:  //structors

  BitExpandLeft();

  //generated destructor is fine


public:  //sole instance

  static BitExpandLeft const instance;

};


//////////////////////////////////////////////////////////////////////////////
//  BitExpandRight
//
//  sets bit at 1 on the right of already set bits
//
class BitExpandRight : public ByteMap
{

private:  //structors

  BitExpandRight();

  //generated destructor is fine


public:  //sole instance

  static BitExpandRight const instance;

};




} } } //namespace avs::text::aliaser

#endif //__AVS_TEXT_ALIASER_BITEXPAND_H__
