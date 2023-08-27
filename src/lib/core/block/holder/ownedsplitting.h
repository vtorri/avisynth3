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


#ifndef __AVS_BLOCK_HOLDER_OWNEDSPLITTING_H__
#define __AVS_BLOCK_HOLDER_OWNEDSPLITTING_H__

//avisynth includes
#include "splittag.h"
#include "ownedbase.h"

//stl include
#include <memory>        //for shared_ptr


namespace avs { namespace block { namespace holder {



////////////////////////////////////////////////////////////////////////////////////
//  holder::OwnedSplitting
//
//  owned holder implementation to handle splits
//
class OwnedSplitting : public OwnedHolder
{

  int size_;
  int offset_;                           //offset of mem piece
  std::shared_ptr<OwnedHolder> holder_;  //underlying owned mem holder


public:  //structors

  OwnedSplitting(std::shared_ptr<OwnedHolder> const& holder, int splitSize, split::Left);
  OwnedSplitting(std::shared_ptr<OwnedHolder> const& holder, int splitSize, split::Right);

  OwnedSplitting(OwnedSplitting const& other, int splitSize, split::Left);
  OwnedSplitting(OwnedSplitting const& other, int splitSize, split::Right);

  //generated destructor is fine


public:  //Holder interface

  virtual int Size() const { return size_; }
  virtual BYTE * Get() const { return holder_->Get() + offset_; }
  virtual PEnvironment const& GetEnvironment() const { return holder_->GetEnvironment(); }
  virtual bool Unique() const { return holder_->Unique(); }

  virtual std::shared_ptr<OwnedHolder> Split(int splitSize, std::shared_ptr<OwnedHolder>& self) const;

};



} } } //namespace avs::block::holder

#endif //__AVS_BLOCK_HOLDER_OWNEDSPLITTING_H__
