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


#ifndef __AVS_BLOCK_DELETER_BASE_H__
#define __AVS_BLOCK_DELETER_BASE_H__


namespace avs { namespace block { namespace deleter {



class Base
{

  int size_;


public:  //structors

  Base(int size) : size_( std::max(size, 0) ) { }
  //generated copy constructor and destructor are fine


public:  //read access

  int size() const { return size_; }


public:  //some definitions for subclasses

  enum { Align = 1 };               //default alignment guarantee (ie not aligned)
  typedef unsigned char BYTE;       //BYTE typedef

};



} } } //namespace avsv::block::deleter

#endif //__AVS_BLOCK_DELETER_BASE_H__
