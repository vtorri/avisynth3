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


#ifndef __AVS_MEMORY_HOLDER_SPLITTING_H__
#define __AVS_MEMORY_HOLDER_SPLITTING_H__

//avisynth includes
#include "../holder.h"


namespace avs { namespace memory { namespace holder {



////////////////////////////////////////////////////////////////////////////////////
//  holder::Splitting
//
//
//
class Splitting : public Holder
{

  //inner type
  struct SplitCounter
  {
    std::shared_ptr<void> counter_;

    int Count() const { return counter_.use_count(); }
  };

  //members
  int32 size_;
  int32 offset_;
  PHolder holder_;            //underlying split memory holder
  SplitCounter counter_;      //split counter, needed for accurate implementation of Unique()


private:  //structors

  Splitting(int32 size, int32 offset, PHolder const& holder, SplitCounter const& counter);

  Splitting(int32 size, int32 offset, Splitting const& other);

  //generated destructor is fine


public:  //Holder observers

  virtual int32 Size() const { return size_; }
  virtual uint8 * Get() const { return holder_->Get() + offset_; }

  virtual bool Unique() const;


public:  //special stuff

  virtual PHolder Split(int32 splitSize, PHolder& self) const;

  virtual PMemManager const& GetMemoryManager() const { return holder_->GetMemoryManager(); }


public:  //called by memory::Holder to implement its Split for other Holder subclasses

  static PHolder InitialSplit(int32 splitSize, PHolder& self);

};



} } } //namespace avs::memory::holder

#endif //__AVS_MEMORY_HOLDER_SPLITTING_H__
