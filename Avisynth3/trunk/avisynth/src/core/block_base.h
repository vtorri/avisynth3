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


#ifndef __AVS_ABSTRACTBLOCK_H__
#define __AVS_ABSTRACTBLOCK_H__


#include <boost/shared_ptr.hpp>  //for shared_ptr


namespace avs { namespace block {


//declarations and typedefs
class Recycler;

enum { Align = 16 };                    //alignment of memory allocated

typedef unsigned char BYTE;
typedef boost::shared_ptr<BYTE> Block;  



template <class Deleter> class base
{
  
  Block block_;


public:  //typedefs

  typedef BYTE BYTE;
  typedef Recycler Recycler;


public:  //alignement stuff

  //alignment of memory allocated
  enum { Align = block::Align };

  //alignment helper methods
  static int AlignValue(int value) { return (value + Align - 1) & -Align; }
  static bool IsAligned(int value) { return value % Align == 0; }


public:  //constructors

  base(Deleter const& deleter)
    : block_( Recycler().Acquire(deleter.size_), deleter ) { }

  //generated copy constructor is fine


public:  //assignment

  //generated operator= is fine

  void swap(base<Deleter>& other) { block_.swap( other.block_ ); }


public:  //queries

  BYTE * get() const { return block_.get(); }

  bool unique() const { return block_.unique(); }

  int size() const { return GetDeleter().size_; }


protected:

  Deleter const& GetDeleter() const { return *boost::get_deleter<Deleter>(block_); }

};



} } //namespace avs::block

#endif //__AVS_ABSTRACTBLOCK_H__