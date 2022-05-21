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


#ifndef __AVS_THRESHOLD_H__
#define __AVS_THRESHOLD_H__

//avisynth include
#include "../core/exception/generic.h"


namespace avs {




class Threshold
{

  int threshold_;  //underlying threshold value


public:  //constructors

  Threshold(int threshold)
    : threshold_( threshold )
  {
    if ( threshold < 0 )
      throw exception::Generic("a threashold must be positive");
  }

  Threshold(Threshold const& other)
    : threshold_( threshold_ ) { }


public:  //threshold methods

  bool IsReached(int value) const { return threshold_ <= value; }
  bool IsBelow(int value) const { return value < threshold_; }

  bool IsAbsReached(int value) const { return IsReached( std::max(value, -value) ); }
  bool IsAbsBelow(int value) const { return IsBelow( std::max(value, -value) ); }

  bool AreClose(int a, int b) const { return IsAbsBelow(a - b); }

};


} //namespace avs

#endif //__AVS_THRESHOLD_H__
