// Avisynth v3.0 alpha.  Copyright 2003 Ben Rudiak-Gould et al.
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


#ifndef __AVS_BLOCK_BASE_H__
#define __AVS_BLOCK_BASE_H__

//avisynth include
#include "align.h"

//boost include
#include <boost/shared_ptr.hpp>  //for shared_ptr


namespace avs { namespace block {


//declaration
class Recycler;



/////////////////////////////////////////////////////////////////////////////////////
//  block::base<Deleter>
//
//
//
template <class Deleter> class base
{
  
public:  //typedefs

  typedef Recycler Recycler;
  typedef unsigned char BYTE;


private:  //member

  boost::shared_ptr<unsigned char> block_;


public:  //structors

  base(Deleter const& deleter)
    : block_( Recycler().Acquire(deleter.size()), deleter ) { }

  //generated copy constructor and destructor are fine


public:  //assignment

  //generated operator= is fine

  void swap(base<Deleter>& other) { block_.swap( other.block_ ); }


public:  //queries

  BYTE * get() const { return block_.get(); }

  bool unique() const { return block_.unique(); }

  int size() const { return GetDeleter().size(); }


protected:

  Deleter const& GetDeleter() const { return *boost::get_deleter<Deleter>(block_); }

};



} } //namespace avs::block

#endif //__AVS_BLOCK_BASE_H__