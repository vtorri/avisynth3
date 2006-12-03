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


#ifndef __AVS_MEMORY_HOLDER_STANDARD_H__
#define __AVS_MEMORY_HOLDER_STANDARD_H__

//avisynth includes
#include "../holder.h"


namespace avs { namespace memory { namespace holder {



////////////////////////////////////////////////////////////////////////////////////
//  holder::Standard
//
//  avs standard implementation of the Holder interface
//  allocates memory through a Memory Manager
//
class Standard : public Holder
{

  int32 size_;
  uint8 * ptr_;
  PMemManager manager_;


public:  //structors

  Standard(int32 size, PMemManager const& manager);
  virtual ~Standard();


public:  //Holder observers

  virtual int32 Size() const { return size_; }
  virtual uint8 * Get() const { return ptr_; }

  virtual bool Unique() const { return true; }


public:  //special stuff

  virtual PHolder Split(int32 splitSize, PHolder& self) const;

  virtual PMemManager const& GetMemoryManager() const { return manager_; }

};



} } } //namespace avs::memory::holder

#endif //__AVS_MEMORY_HOLDER_STANDARD_H__
