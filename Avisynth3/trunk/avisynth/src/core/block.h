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


#ifndef __AVS_BLOCK_H__
#define __AVS_BLOCK_H__


//boost include
#include <boost/shared_ptr.hpp>


namespace avs {


//typedef
typedef unsigned char BYTE;



///////////////////////////////////////////////////////////////////////////////////////////
//  Block
//
//  Holds an aligned block of memory, with automatic deallocation.
//
//  Two behaviors are provided :
//    - basic behavior : 
//         allocation done at construction and deallocation at destruction
//    - recycling behavior :
//         a list of block of memory (and their size) is maintained.
//      At construction it attempts to reuse one from this list if size match.
//      At destruction, the block of memory is stored (back) into the list.
//
//  The recycling behavior is fully synchronised and can serve multiple threads at once.
//
//
//  Problem: The recycling list only grows and never release memory,
//             even when some sizes are never requested anymore....
//           
class Block
{

  boost::shared_ptr<BYTE> block;  //underlying shared_ptr


public:  

  //alignment of memory allocated by Blocks
  enum { Align = 16 };

  //recycle struct, used as a signature for the recycling constructor
  struct recycle { };


public:  //constructors

  Block(int size);               //basic constructor

  Block(int size, recycle);      //recycling constructor

  Block(Block const& other)      //copy constructor
    : block( other.block ) { }


public:  //assignment

  Block& operator=(Block const& other) { block = other.block; return *this; }

  void swap(Block& other) { block.swap( other.block ); }


public:  //reset methods

  void reset(int size) { Block(size).swap(*this); }
  void reset(int size, recycle r) { Block(size, r).swap(*this); }


public:  //read access

  BYTE * get() const { return block.get(); }
  int size() const;

  bool unique() const { return block.unique(); }


public:  //helper methods

  static int AlignValue(int value) { return (value + Align - 1) & -Align; }

  static bool IsAligned(int value) { return value % value == 0; }


private:  //implementation details 
  
  class Manager;      //manages recycling and sizes of Blocks


  static BYTE * New(int& size);

  static void Delete(void * ptr);  //no throw
  static void Recycle(void * ptr); //no throw

};//Block






} //namespace avs

#endif //__AVS_BLOCK_H__