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


#ifndef __AVS_BYTEMAP_H__
#define __AVS_BYTEMAP_H__

//boost include
#include <boost/utility.hpp>

//assert include
#include <assert.h>


namespace avs {



//////////////////////////////////////////////////////////////////////////////////////////
//  ByteMap
//
//  Maps byte values into processed (byte) values
//  This class is meant to be used as a base class, subclass constructor doing map init 
//  
class ByteMap : private boost::noncopyable
{

  unsigned char map_[256];


protected:  //structors

  ByteMap() { }

  //generated destructor is fine


public:  //read access

  unsigned char const& operator[](int i) const { assert(i >= 0 && i < 256); return map_[i]; }
  unsigned char const * data() const { return map_; }


protected:  //write access

  unsigned char * data() { return map_; }
  unsigned char & operator[](int i) { return map_[i]; }

};


} //namespace avs 

#endif //__AVS_BYTEMAP_H__
