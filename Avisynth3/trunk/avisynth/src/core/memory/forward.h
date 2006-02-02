// Avisynth v3.0 alpha.  Copyright 2003-2006 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_MEMORY_FORWARD_H__
#define __AVS_MEMORY_FORWARD_H__

//avisynth include
#include "align.h"             //for Align

//boost forward declaration
namespace boost { template <typename T> class shared_ptr; }


namespace avs { namespace memory {


//declarations
class Holder;
class Blitter;
class Manager;
template <int align> class block_;

//typedef
typedef block_<Align> Block;

//ptr typedefs
typedef boost::shared_ptr<Heap> PHeap;
typedef boost::shared_ptr<Manager> PMemManager;
typedef boost::shared_ptr<Holder const> PHolder;
typedef boost::shared_ptr<Blitter const> PBlitter;


} } //namespace avs::memory

#endif //__AVS_MEMORY_FORWARD_H__
