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

#include "bufferwindow.h"
#include "videoinfo.h"

/**********************************************************************************************/
/************************************ VideoFrameBuffer ****************************************/
/**********************************************************************************************/







VideoFrameBuffer::MemoryPiece::MemoryPiece(int _size) : size(_size)
{
  AllocationVector::iterator it = alloc.begin();
  while( it != alloc.end() && it->size != size ) { ++it; }
  if (it == alloc.end() )
    data.reset( new BYTE[size] );
  else {
    data = it->data;
    alloc.erase(it);
  }
}



AlphaForwardingBuffer::AlphaForwardingBuffer(int row_size, int height)
: ForwardingBuffer(row_size, FRAME_ALIGN)
{
  int size = SizeFor(pitch, height);
  if (largest.empty() || largest->GetSize() < size)
    largest = new SharedAlphaBuffer(size);
  
  forwardTo = largest;
}



/**********************************************************************************************/
/************************************** BufferWindow ******************************************/
/**********************************************************************************************/




void BufferWindow::NewWindow(int _row_size, int _height)
{
  row_size = _row_size;
  height = _height;
  vfb = new PlaneFrameBuffer(row_size, height);
  offset = FirstOffset(vfb);
}

void BufferWindow::NewSolidWindow(int _row_size, int _height, int pitch, PVideoFrameBuffer solid, int solidOffset)
{
  row_size = _row_size;
  height = _height;  
  vfb = new PlaneForwardingBuffer(pitch, solidOffset, height, solid);
  offset = 0;
}

void BufferWindow::NewAlphaWindow(int _row_size, int _height)
{
  row_size = _row_size;
  height = _height;
  vfb = new AlphaForwardingBuffer(row_size, height);
  offset = FirstOffset(vfb);
}


BYTE* BufferWindow::GetWritePtr()
{  
  BYTE * result = vfb->GetWritePtr(); 
  //returns NULL if we are not allowed to write
  if (result == NULL)
  {
    VideoFrameBuffer * new_vfb = new PlaneFrameBuffer(row_size, height);
    int new_offset = FirstOffset(new_vfb);
    IScriptEnvironment::BitBlt(new_vfb->GetWritePtr() + new_offset, new_vfb->GetPitch(), vfb->GetReadPtr() + offset, vfb->GetPitch(), row_size, height);
    vfb = new_vfb;
    offset = new_offset;

    result = = vfb->GetWritePtr();     
  }

  return result + offset;
} 



void BufferWindow::Copy(const BufferWindow& other, int left, int top)
{
  _ASSERTE(this != &other);

  Vector top_left = StickToWindow(left, top);
  Vector bottom_right = StickToWindow(left + other.row_size, top + other.height);

  BYTE * dstp = GetWritePtr(); 
  int dst_pitch = GetPitch();
  const BYTE * srcp = other.GetReadPtr();
  int src_pitch = other.GetPitch()

  int copy_row_size = bottom_right.x - top_left.x;
  int copy_height = bottom_right.y - top_left.y;

  if (copy_height > 0 && copy_row_size > 0) //only work if necessary 
    IScriptEnvironment::BitBlt(
      dstp + top_left.x + dst_pitch * top_left.y,
      dst_pitch,
      srcp + top_left.x - left + src_pitch * (top_left.y - top),
      src_pitch,
      copy_row_size,
      copy_height
    );
                                         
}

void BufferWindow::Crop(int left, int right, int top, int bottom)
{
  BufferWindow saved = *this;   //save actual position
  //updating parameters
  row_size -= left + right;
  height -= top + bottom;
  offset += VectorToWindowOffset(left, top);
  //three conditions where needed to reallocate the Buffer
  int pitch = GetPitch();
  if ( row_size > pitch || offset < 0 || offset + height * pitch >= vfb->GetSize() )
  {
    vfb = new PlaneFrameBuffer(row_size, height);  //new buffer
    offset = FirstOffset(vfb);
    Copy(saved, -left, -top);                      //copy saved data at the right place
  }
}

void BufferWindow::Blend(const BufferWindow& other, float factor)
{
  _ASSERTE(this != &other);
  if (factor <= 0) 
    return;         //no change
  if (factor >= 1)
    *this = other;  //we replace this window by the other one
  else {
    //TODO: Code ME !!
  }
}

void BufferWindow::FlipVertical()
{
  BufferWindow result(row_size, height);

  IScriptEnvironment::BitBlt(result.GetWritePtr(), result.GetPitch(), 
    GetReadPtr() + (height - 1)*GetPitch(), -GetPitch(), row_size, height);
  *this = result;
}


