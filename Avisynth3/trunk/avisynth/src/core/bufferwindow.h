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
#include "dimension.h"
#include "ownedblock.h"
#include "window_ptr.h"


namespace avs {



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


public:  //sstructors

  //normal constructor
  BufferWindow(Dimension const& dim, PEnvironment const& env);
 
  //generated copy constructor and destructor are fine
  

public:  //assignment

  //generated operator= is fine

  void swap(BufferWindow& other);  //no throw


public:  //access

  PEnvironment const& GetEnvironment() const { return buffer_.GetEnvironment(); }
  Dimension const& GetDimension() const { return dim_; }

  BYTE const * read() const { return buffer_.get() + offset_; }
  BYTE * write();

  int pitch() const { return pitch_; }
  int width() const { return dim_.GetWidth(); }
  int height() const { return dim_.GetHeight(); }


  CWindowPtr Read() const { return CWindowPtr( read(), pitch(), width(), height() ); }
  WindowPtr Write() { BYTE * ptr = write(); return WindowPtr( ptr, pitch(), width(), height() ); }

  CWindowPtr ReadFromBottom() const { return CWindowPtr( read() + pitch() * (height() - 1), -pitch(), width(), height() ); }
  WindowPtr WriteFromBottom() const { BYTE * ptr = write(); return WindowPtr( ptr + pitch() * (height() - 1), -pitch(), width(), height() ); }  


public:  //comparison operators

  bool operator==(BufferWindow const& other) const { return buffer_ == other.buffer_ && offset_ == other.offset_ && dim_ == other.dim_; }
  bool operator!=(BufferWindow const& other) const { return buffer_ != other.buffer_ || offset_ != other.offset_ || dim_ != other.dim_; }


public:  

  //buffer resizing method
  //will try to avoid buffer reallocation when possible
  void ChangeSize(Vecteur const& topLeft, Vecteur const& bottomRight);

  //copy other into self at the given coords (which can be negatives)
  //only overlap is copied, no effect if there is none
  void Copy(BufferWindow other, Vecteur const& coords); 
  
};//BufferWindow


} //namespace avs

#endif //__AVS_BUFFERWINDOW_H__