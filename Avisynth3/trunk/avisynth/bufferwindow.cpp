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




AbstractVideoFrameBuffer::AbstractVideoFrameBuffer(int row_size, int _align) 
: align( (_align < 0)? -_align : max(_align, FRAME_ALIGN) ), pitch(Aligned(row_size)) { }



VideoFrameBuffer::MemoryPiece::MemoryPiece(int _size) : size(_size)
{
  AllocationVector::iterator it = alloc.begin();
  while( it != alloc.end() && it->size != size ) { ++it; }
  if (it == alloc.end() )
    data = new BYTE[size];
  else {
    data = it->data;
    alloc.erase(it);
  }
}



AlphaVideoFrameBuffer::AlphaVideoFrameBuffer(int row_size, int height, int _align)
: AbstractVideoFrameBuffer(row_size, _align)
{
  int size = SizeFor(pitch, height, align);
  if (largest && largest->GetSize() >= size)
    buffer = largest;
  else {
    largest = buffer = new VideoFrameBuffer(row_size, height, _align);
    //Fill it with 255
    memset(buffer->GetWritePtr(), 255, buffer->GetSize());
  }
}



/**********************************************************************************************/
/************************************** BufferWindow ******************************************/
/**********************************************************************************************/


BYTE* BufferWindow::GetWritePtr()
{  
  BYTE * result = vfb->GetWritePtr(); 
  //returns NULL if we are not allowed to write
  if (result == NULL)
  {
    VideoFrameBuffer * new_vfb = new VideoFrameBuffer(row_size, height, -vfb->GetAlign());
    int new_offset = new_vfb->FirstOffset();
    IScriptEnvironment::BitBlt(new_vfb->GetWritePtr() + new_offset, new_vfb->GetPitch(), vfb->GetReadPtr() + offset, vfb->GetPitch(), row_size, height);
    vfb = new_vfb;
    offset = new_offset;
  }

  return result + offset;
} 


BufferWindow::BufferWindow(const BufferWindow& other, int left, int right, int top, int bottom)
: row_size(other.row_size - left - right), height(other.height - top - bottom),
    offset(other.offset + left + top*other.GetPitch()), vfb(other.vfb)
{
  //three conditions where needed to reallocate the Buffer
  if (row_size > GetPitch() || offset < 0 || offset + height * GetPitch() >= vfb->GetSize())
  {
    vfb = new VideoFrameBuffer(row_size, height, -vfb->GetAlign());
    offset = vfb->FirstOffset();
    Copy(other, -left, -top);
  }
}

void BufferWindow::Copy(const BufferWindow& other, int left, int top)
{
  _ASSERTE(this != &other);
  BYTE * dst = GetWritePtr();  //has to be done first since it can change the pitch of self
  const BYTE * src = other.GetReadPtr();
  int copy_offset = 0, copy_offset_other = 0;
  int pitch = GetPitch();
  int pitch_other = other.GetPitch();
  int copy_row_size, copy_height;
  if (left < 0) {
    copy_row_size = min(row_size, other.row_size + left);
    copy_offset_other -= left;
  } else {
    copy_row_size = min(row_size - left, other.row_size);
    copy_offset += left;
  }
  if (top < 0) {
    copy_height = min(height, other.height + top);
    copy_offset_other -= top*pitch_other;
  } else {
    copy_height = min(height - top, other.height);
    copy_offset += top*pitch;
  }
  if (copy_height > 0 && copy_row_size > 0) //these checks should be moved in BitBlt
    IScriptEnvironment::BitBlt(dst + copy_offset, pitch, src + copy_offset_other, pitch_other, copy_row_size, copy_height);                                          
}

void BufferWindow::Blend(const BufferWindow& other, float factor)
{
  static const AvisynthError UNMATCHING_SIZE("Blend Error: Height or Width don't match");
  _ASSERTE(this != &other);
  if (row_size != other.row_size || height != other.height)
    throw UNMATCHING_SIZE;
  if (factor <= 0) 
    return;         //no change
  if (factor >= 1)
    *this = other;  //we replace this window by the other one
  else {
    //TODO: Code ME !!
  }
}
