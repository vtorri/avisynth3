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
  
  Dimension dimension_;        //dimension of the window
  int pitch_;
  int offset;                  //offset from buffer start to window start
  OwnedBlock buffer;


public:  //constructors

  //normal constructor
  BufferWindow(Dimension const& dimension, PEnvironment env);
 
  //copy constructor
  BufferWindow(BufferWindow const& other);
  

public:  //assignment

  BufferWindow& operator=(BufferWindow const& other);

  void swap(BufferWindow& other);


public:  //access

  PEnvironment GetEnvironment() const { return buffer.GetEnvironment(); }
  Dimension const& GetDimension() const { return dimension_; }

  BYTE const * read() const { return buffer.get() + offset; }
  BYTE * write();

  int pitch() const { return pitch_; }
  int width() const { return dimension_.GetWidth(); }
  int height() const { return dimension_.GetHeight(); }


  CWindowPtr Read() const { return CWindowPtr( read(), pitch(), width(), height() ); }
  WindowPtr Write() { return WindowPtr( write(), pitch(), width(), height() ); }


public:  

  //buffer resizing method
  //will try to avoid buffer reallocation when possible
  void ChangeSize(Vecteur const& topLeft, Vecteur const& bottomRight);

  //copy other into self at the given coords (which can be negatives)
  //only overlap is copied, no effect if there is none
  void Copy(BufferWindow const& other, Vecteur const& coords); 
  
  //blend other into self at the given factor
  //unspecified behavior if dimensions don't match
  void Blend(BufferWindow const& other, float factor);

  void FlipVertical();
  void FlipHorizontal(int grain = 1);

  void TurnLeft(int grain = 1);
  void TurnRight(int grain = 1);

};//BufferWindow


} //namespace avs

#endif //__AVS_BUFFERWINDOW_H__