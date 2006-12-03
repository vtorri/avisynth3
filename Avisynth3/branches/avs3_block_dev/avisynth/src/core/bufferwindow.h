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


#ifndef __AVS_BUFFERWINDOW_H__
#define __AVS_BUFFERWINDOW_H__

//avisynth includes
#include "blitter.h"
#include "window_ptr.h"
#include "geometry/dimension.h"         //so Dimension is defined

//stlport include
#include <cassert>


namespace avs { namespace bw { 


//declaration  
struct SizeChanger; 



////////////////////////////////////////////////////////////////////////////////////////////
//  bw::misAligned<align>
//
//  helper struct to detect misalignment
//
template <int align> struct misAligned
{
  template <typename BW> bool operator()(BW const& bw) const
  {
    return std::max(bw.Pitch(), -bw.Pitch()) % align != 0       //pitch non aligned
        || reinterpret_cast<size_t>( bw.read() ) % align != 0;     //data non aligned
  }
};


//////////////////////////////////////////////////////////////////////////////////////////////
//  bw::misAligned<1>
//
//  border case specialisation of the above
//
template <> struct misAligned<1>
{
  template <typename BW> bool operator()(BW const& bw) const
  {
    return false;
  }
};


/////////////////////////////////////////////////////////////////////////////////////////////
//  bw::misGuarded<guard>
//
//  helper struct to detect misguarding
//
template <int guard> struct misGuarded
{
  template <typename BW> bool operator()(BW const& bw) const
  {
    return bw.MinOffset() < guard                            //not enough head space
        || bw.buffer_.Size() < bw.MaxOffset() + guard;       //not enough toe space        
  }
};


////////////////////////////////////////////////////////////////////////////////////////////////
//  bw::misGuarded<0>
//
//  specialisation on border case of the above
//
template <> struct misGuarded<0>
{
  template <typename BW> bool operator()(BW const& bw) const
  {
    return false;
  }
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
  typedef typename Buffer::Creator BufferCreator;
  typedef buffer_window<align, guard, Buffer> BufferWindowType;

  friend struct bw::SizeChanger;   //need internal knowledge to work
  friend struct bw::misGuarded<guard>;

  //needed for the converting constructor to access other's members
  template <int alignOther, int guardOther, class BufferOther> friend class buffer_window;


public:  //structors

  //normal constructor
  buffer_window(Dimension const& dim, BufferCreator const& create)
    : dim_( dim )
    , pitch_( utility::RoundUp<Align>(Width()) )
    , offset_( Guard )
    , buffer_( create(Pitch() * Height() + Guard * 2) ) { }
 
  //constructor using a given buffer
  template <class BufferOther>
  buffer_window(Dimension const& dim, BufferOther const& buffer, int offset)
    : dim_( dim )
    , pitch_( utility::RoundUp<Align>(Width()) )
    , offset_( offset )
    , buffer_( buffer )
  {
    assert( IsLegal() );

    ReAlign();
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

    ReAlign();
  }


  //conversion from another buffer_window type
  template<int alignOther, int guardOther, class BufferOther>
  buffer_window(buffer_window<alignOther, guardOther, BufferOther> const& other)
    : dim_( other.dim_ )
    , pitch_( other.pitch_ )
    , offset_( other.offset_ )
    , buffer_( other.buffer_ )
  {
    ReAlign();
  }

  //generated copy constructor and destructor are fine
  

public:  //assignment

  //generated operator= is fine

  void swap(BufferWindowType& other)  //no throw
  {
    using std::swap;

    swap(dim_, other.dim_);
    swap(pitch_, other.pitch_);
    swap(offset_, other.offset_);
    swap(buffer_, other.buffer_);
  }


public:  //access

  PEnvironment const& GetEnvironment() const { return buffer_.GetEnvironment(); }
  Dimension const& GetDimension() const { return dim_; }

  BYTE const * read() const { return buffer_.Get() + offset_; }
  BYTE * write()
  {
    if ( ! buffer_.Unique() )                      //if data is shared
      SelfBlit();                                  //blit it so we become sole owner
    return buffer_.Get() + offset_;
  }

  int Pitch() const { return pitch_; }
  int Width() const { return dim_.GetWidth(); }
  int Height() const { return dim_.GetHeight(); }
  int Size() const { return std::max(Pitch(), -Pitch()) * Height(); }

  //window_ptr methods
  CWindowPtr Read() const { return CWindowPtr( read(), Pitch(), Width(), Height() ); }
  WindowPtr Write() { BYTE * ptr = write(); return WindowPtr( ptr, Pitch(), Width(), Height() ); }


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

  int MinOffset() const { return pitch_ > 0 ? offset_ : offset_ + pitch_ * (Height() - 1); }
  int MaxOffset() const { return pitch_ > 0 ? offset_ + pitch_ * Height() : offset_ - pitch_; }

  //checks that the data is inside the buffer
  bool IsLegal() const { return ! dim_.empty() && 0 <= MinOffset() && MaxOffset() <= buffer_.Size(); }

  void ReAlign()
  {
    if ( bw::misAligned<Align>()(*this) || bw::misGuarded<Guard>()(*this) )
      SelfBlit();
  }

  void SelfBlit()
  {
    BufferWindowType temp( dim_, BufferCreator(buffer_) );  //make a new buffer
    Blitter::Get().Blit(Read(), temp.Write(), dim_);        //blit the data into it
    swap(temp);                                             //and replace self
  }

};


//global scope swap
template <int align, int guard, class Buffer>
inline void swap(buffer_window<align, guard, Buffer>& left, buffer_window<align, guard, Buffer>& right) { left.swap(right); }


} //namespace avs

#endif //__AVS_BUFFERWINDOW_H__
