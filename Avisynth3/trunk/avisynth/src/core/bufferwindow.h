// Avisynth v3.0 alpha.  Copyright 2004 Ben Rudiak-Gould et al.
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
#include "dimension.h"
#include "ownedblock.h"
#include "window_ptr.h"
#include "block/align.h"
#include "runtime_environment.h"


namespace avs {


//declaration
namespace bw { struct SizeChanger; }



/////////////////////////////////////////////////////////////////////////////////////////////
//  BufferWindow
//
//
//
class BufferWindow
{

  Dimension dim_;              //dimension of the window
  int pitch_;
  int offset_;                 //offset from buffer start to window start
  OwnedBlock buffer_;

  friend struct bw::SizeChanger;   //need internal knowledge to work


public:  //structors

  //normal constructor
  BufferWindow(Dimension const& dim, PEnvironment const& env)
    : dim_( dim )
    , pitch_( block::AlignValue(width()) )
    , offset_( block::Align )
    , buffer_( env->NewOwnedBlock(pitch() * height() + block::Align * 2, true) ) { }
 
  //generated copy constructor and destructor are fine
  

public:  //assignment

  //generated operator= is fine

  void swap(BufferWindow& other)  //no throw
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
    if ( ! buffer_.unique() )
    {
      PEnvironment const& env = GetEnvironment();
      BufferWindow buf(dim_, env);

      env->GetBlitter()(*this, buf);

      *this = buf;
    }
    return buffer_.get() + offset_;
  }

  int pitch() const { return pitch_; }
  int width() const { return dim_.GetWidth(); }
  int height() const { return dim_.GetHeight(); }

  //window_ptr methods
  CWindowPtr Read() const { return CWindowPtr( read(), pitch(), width(), height() ); }
  WindowPtr Write() { BYTE * ptr = write(); return WindowPtr( ptr, pitch(), width(), height() ); }


public:  //comparison operators

  bool operator==(BufferWindow const& other) const { return buffer_ == other.buffer_ && offset_ == other.offset_ && dim_ == other.dim_; }
  bool operator!=(BufferWindow const& other) const { return buffer_ != other.buffer_ || offset_ != other.offset_ || dim_ != other.dim_; }

 
};



} //namespace avs

#endif //__AVS_BUFFERWINDOW_H__
