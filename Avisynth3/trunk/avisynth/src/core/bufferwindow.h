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


namespace avs {


//declaration
namespace bw { struct SizeChanger; }



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

  friend struct bw::SizeChanger;   //need internal knowledge to work
  //needed for the converting constructor to access other's members
  template <int alignOther, int guardOther, class BufferOther> friend class buffer_window;


public:  //declarations and typedef

  enum { Align = align, Guard = guard };

  typedef Buffer BufferType;
  typedef buffer_window<align, guard, Buffer> BufferWindowType;


public:  //structors

  //normal constructor
  buffer_window(Dimension const& dim, typename BufferType::Creator const& create)
    : dim_( dim )
    , pitch_( RoundUp<Align>(width()) )
    , offset_( Guard )
    , buffer_( create(pitch() * height() + Guard * 2, true) ) { }
 
  //constructor using a given buffer
  template <class BufferOther>
  buffer_window(Dimension const& dim, BufferOther const& buffer, int offset)
    : dim_( dim )
    , pitch_( RoundUp<Align>(width()) )
    , offset_( offset )
    , buffer_( buffer )
  {
    assert( IsLegal() );

    if ( MisAligned() )       //if given values make self not respect align and guard contracts
      ReAlign();              //realign (blit to correct)
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

    if ( MisAligned() && std::max(pitch_, -pitch_) % Align != 0 )    //if do not respect guard and align contracts
      ReAlign();                                                     //blit to correct
  }


  //conversion from another buffer_window type
  template<int alignOther, int guardOther, class BufferOther>
  buffer_window(buffer_window<alignOther, guardOther, BufferOther> const& other)
    : dim_( other.dim_ )
    , pitch_( other.pitch_ )
    , offset_( other.offset_ )
    , buffer_( other.buffer_ )
  {
    if ( MisAligned() && pitch_ % Align != 0 )    //if do not respect guard and align contracts
      ReAlign();                                  //blit to correct
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


public:  //access

  PEnvironment const& GetEnvironment() const { return buffer_.GetEnvironment(); }
  Dimension const& GetDimension() const { return dim_; }

  BYTE const * read() const { return buffer_.get() + offset_; }
  BYTE * write()
  {
    if ( ! buffer_.unique() )                      //if data is shared
      ReAlign();                                   //blit it so we become sole owner
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
 

private:  //alignment stuff

  int MinOffset() const { return pitch_ > 0 ? offset_ : offset_ + pitch_ * (height() - 1); }
  int MaxOffset() const { return pitch_ > 0 ? offset_ + pitch_ * height() : offset_ - pitch_; }

  //checks that the data is inside the buffer
  bool IsLegal() const { return ! dim_.empty() && 0 <= MinOffset() && MaxOffset() <= buffer_.size(); }

  bool MisAligned() const
  {
    return MinOffset() < Guard                           //not enough head space
        || int(buffer_.get() + offset_) % Align != 0     //data misaligned
        || buffer_.size() < MaxOffset() + Guard;         //not enough toe space
  }

  void ReAlign()
  {
    PEnvironment const& env = GetEnvironment();      //fetch owning env

    BufferWindowType temp(dim_, env);                //make a new buffer
    Blitter::Get()(Read(), temp.Write(), dim_);      //blit the data into it
    *this = temp;                                    //and replace self
  }

};


//global scope swap
template <int align, int guard, class Buffer>
inline void swap(buffer_window<align, guard, Buffer>& left, buffer_window<align, guard, Buffer>& right) { left.swap(right); }


} //namespace avs

#endif //__AVS_BUFFERWINDOW_H__
