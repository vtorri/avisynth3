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


#ifndef __AVS_BUFFERWINDOW_H__
#define __AVS_BUFFERWINDOW_H__

//avisynth includes
#include "blitter.h"
#include "window_ptr.h"
#include "geometry/dimension.h"

//assert include
#include "assert.h"


namespace avs { namespace bw { 


//declaration  
struct SizeChanger; 



/////////////////////////////////////////////////////////////////////////////////////////////
//  bw::realigner<align, guard>
//
//  helper struct to help  buffer_window enforce its align and guard guarantee
//
//  code has been moved to an external struct so it could be specialised on border cases
//
template <int align, int guard> struct realigner
{

  template <class Buffer>
  void operator()(buffer_window<align, guard, Buffer> & bw) const
  {
    if (  std::max(bw.pitch_, -bw.pitch_) % align != 0      //pitch non aligned
       || int(bw.buffer_.get() + bw.offset_) % align != 0   //data non aligned
       || bw.MinOffset() < guard                            //not enough head space
       || bw.buffer_.size() < bw.MaxOffset() + guard        //not enough toe space
        )
      bw.SelfBlit();
  }
};


////////////////////////////////////////////////////////////////////////////////////////////////
//  bw::realigner<1, 0>
//
//  specialisation for no alignment, no guard, ie no realigning ever
//
template <> struct realigner<1, 0>
{
  template <class Buffer>
  void operator()(buffer_window<1, 0, Buffer> & buffer) const { }
};


}  //namespace bw



/////////////////////////////////////////////////////////////////////////////////////////////
//  buffer_window<int align, int guard, class Buffer>
//
//
//
template <int align, int guard, class Buffer> 
class buffer_window
{

  Dimension dim_;              //dimension of the window
  int pitch_;
  int offset_;                 //offset from buffer start to window start
  Buffer buffer_;


public:  //declarations and typedef

  enum { Align = align, Guard = guard };

  typedef Buffer BufferType;
  typedef buffer_window<align, guard, Buffer> BufferWindowType;

  friend struct bw::SizeChanger;   //need internal knowledge to work
  friend struct bw::realigner<align, guard>;

  //needed for the converting constructor to access other's members
  template <int alignOther, int guardOther, class BufferOther> friend class buffer_window;


public:  //structors

  //normal constructor
  buffer_window(Dimension const& dim, typename BufferType::Creator const& create)
    : dim_( dim )
    , pitch_( RoundUp<Align>(width()) )
    , offset_( Guard )
    , buffer_( create(pitch() * height() + Guard * 2, true) ) { }
 
  //spawning constructor
  buffer_window(Dimension const& dim, BufferWindowType const& other)
    : dim_( dim )
    , pitch_( RoundUp<Align>(width()) )
    , offset_( Guard )
    , buffer_( other.buffer_.spawn(pitch() * height() + Guard * 2) ) { }

  //constructor using a given buffer
  template <class BufferOther>
  buffer_window(Dimension const& dim, BufferOther const& buffer, int offset)
    : dim_( dim )
    , pitch_( RoundUp<Align>(width()) )
    , offset_( offset )
    , buffer_( buffer )
  {
    assert( IsLegal() );

    bw::realigner<align, guard>()(*this);
  }

  //same as above, but allows custom pitch (possibly negative, but not zero)
  template <class BufferOther>
  buffer_window(Dimension const& dim, BufferOther const& buffer, int offset, int pitch)
    : dim_( dim )
    , pitch_( pitch )
    , offset_( offset )
    , buffer_( buffer )
  {
    assert( IsLegal() && pitch_ != 0 );

    bw::realigner<align, guard>()(*this);
  }


  //conversion from another buffer_window type
  template<int alignOther, int guardOther, class BufferOther>
  buffer_window(buffer_window<alignOther, guardOther, BufferOther> const& other)
    : dim_( other.dim_ )
    , pitch_( other.pitch_ )
    , offset_( other.offset_ )
    , buffer_( other.buffer_ )
  {
    bw::realigner<align, guard>()(*this);
  }

  //generated copy constructor and destructor are fine
  

public:  //assignment

  //generated operator= is fine

  void swap(BufferWindowType& other)  //no throw
  {
    dim_.swap(other.dim_);
    std::swap(pitch_, other.pitch_);
    std::swap(offset_, other.offset_);
    buffer_.swap(other.buffer_);
  }

  //spawn method
  BufferWindowType spawn(Dimension const& dim) const
  {
    return BufferWindowType(dim, *this);
  }


public:  //access

  PEnvironment const& GetEnvironment() const { return buffer_.GetEnvironment(); }
  Dimension const& GetDimension() const { return dim_; }

  BYTE const * read() const { return buffer_.get() + offset_; }
  BYTE * write()
  {
    if ( ! buffer_.unique() )                      //if data is shared
      SelfBlit();                                  //blit it so we become sole owner
    return buffer_.get() + offset_;
  }

  int pitch() const { return pitch_; }
  int width() const { return dim_.GetWidth(); }
  int height() const { return dim_.GetHeight(); }
  int size() const { return std::max(pitch(), -pitch()) * height(); }

  //window_ptr methods
  CWindowPtr Read() const { return CWindowPtr( read(), pitch(), width(), height() ); }
  WindowPtr Write() { BYTE * ptr = write(); return WindowPtr( ptr, pitch(), width(), height() ); }


public:  //comparison operators

  bool operator==(BufferWindowType const& other) const 
  { 
    return buffer_ == other.buffer_ && offset_ == other.offset_ && dim_ == other.dim_; 
  }
  
  bool operator!=(BufferWindowType const& other) const 
  { 
    return buffer_ != other.buffer_ || offset_ != other.offset_ || dim_ != other.dim_; 
  }
 

private:  //implementation details

  int MinOffset() const { return pitch_ > 0 ? offset_ : offset_ + pitch_ * (height() - 1); }
  int MaxOffset() const { return pitch_ > 0 ? offset_ + pitch_ * height() : offset_ - pitch_; }

  //checks that the data is inside the buffer
  bool IsLegal() const { return ! dim_.empty() && 0 <= MinOffset() && MaxOffset() <= buffer_.size(); }

  void SelfBlit()
  {
    BufferWindowType temp(dim_, *this);              //make a new buffer (using spawning contructor)
    Blitter::Get()(Read(), temp.Write(), dim_);      //blit the data into it
    swap(temp);                                      //and replace self
  }

};


//global scope swap
template <int align, int guard, class Buffer>
inline void swap(buffer_window<align, guard, Buffer>& left, buffer_window<align, guard, Buffer>& right) { left.swap(right); }


} //namespace avs

#endif //__AVS_BUFFERWINDOW_H__
