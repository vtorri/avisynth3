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


#ifndef __BUFFERWINDOW_H__
#define __BUFFERWINDOW_H__

#include "avisynth.h"
#include "colorspace.h"
#include <vector>


//simpler replacement for auto_ptr (saves a bool)
//and with a operator=(T*)  (replacement for reset(T*) missing in VC6 STL)
template <class T> class simple_auto_ptr {

  mutable T * ptr;

public:
  simple_auto_ptr() : ptr(NULL) { }
  simple_auto_ptr(T * _ptr) : ptr(_ptr) { }
  simple_auto_ptr(const simple_auto_ptr<T>& other) : ptr(other.release()) { }
  ~simple_auto_ptr() { if (ptr) delete ptr; }

  simple_auto_ptr<T>& operator=(T * _ptr) { if (ptr && ptr != _ptr) delete ptr; ptr = _ptr; return *this; }
  simple_auto_ptr<T>& operator=(const simple_auto_ptr<T>& other) {
    if (this != &other) {
      if (ptr) delete ptr;
      ptr = other.release();
    }
    return *this;
  }
  operator bool() const { return ptr != NULL; }

  T * get() const { return ptr; }  
  T * release() const { T * result = ptr; ptr = NULL; return result; }
  void swap(simple_auto_ptr<T>&) { std::swap<T*>(ptr, other.ptr); }

}; 



class AbstractVideoFrameBuffer : public RefCounted {

protected:

  static int SizeFor(int pitch, int height, int align) { return pitch * height + align * 4; }  

  int pitch;
  int align;

public:
  AbstractVideoFrameBuffer(int row_size, int _align);

  //virtual methods to be implented by subclasses
  virtual const BYTE * GetReadPtr() const = 0;
  virtual BYTE * GetWritePtr() { return NULL; }
  virtual int GetSize() const = 0;

  int GetPitch() const { return pitch; }
  int GetAlign() const { return align; }

  int FirstOffset() const { return int(GetReadPtr()) % align; }

  int Aligned(int dim) { return (dim + align - 1) / align * align; }


};


// VideoFrameBuffer holds information about a memory block which is used
// for video data.  
class VideoFrameBuffer : public AbstractVideoFrameBuffer {  

  typedef simple_auto_ptr<BYTE> Buffer;  //auto_ptr<BYTE> Buffer;
  
  struct MemoryPiece {

    typedef vector<MemoryPiece> AllocationVector;

    static AllocationVector alloc;

    int size;
    Buffer data;

    MemoryPiece(int _size);
    MemoryPiece(const MemoryPiece& other) : size(other.size), data(other.data) { }

  };

  MemoryPiece buffer;

public:
  VideoFrameBuffer(int row_size, int height, int _align)
    : AbstractVideoFrameBuffer(row_size, _align), buffer(SizeFor(row_size, height, _align)) { }
  ~VideoFrameBuffer() { MemoryPiece::alloc.push_back(buffer); }

  const BYTE * GetReadPtr() const { return buffer.data.get(); }
  BYTE * GetWritePtr()  { return IsShared()? NULL : buffer.data.get(); }

  int GetSize() const { return buffer.size; }

  
};


typedef smart_ptr<VideoFrameBuffer> PVideoFrameBuffer;  

//framebuffer for the dummy alpha planes (init to 255)
//so BufferWindow will correctly reallocate the buffer 
class AlphaVideoFrameBuffer : public AbstractVideoFrameBuffer {

  static PVideoFrameBuffer largest;

  PVideoFrameBuffer buffer;

public:
  AlphaVideoFrameBuffer(int row_size, int height, int _align);

  const BYTE * GetReadPtr() const { return buffer->GetReadPtr(); }  
  virtual int GetSize() const { return buffer->GetSize(); }

};






//BufferWindow holds a window in a possibly shared VideoFrameBuffer
//when asked for a write ptr, it will do a (intelligent) copy of the buffer if shared
class BufferWindow {

  PVideoFrameBuffer vfb;
  int offset;
  int row_size, height;
  
public:
  BufferWindow() { } 
  BufferWindow(int _row_size, int _height, int align = FRAME_ALIGN)
    : row_size(_row_size), height(_height), vfb(new VideoFrameBuffer(row_size, height, align)), offset(vfb->FirstOffset()) { }
  //copy constructor
  BufferWindow(const BufferWindow& other) : row_size(other.row_size), height(other.height), vfb(other.vfb), offset(other.offset) { }
  //redimensioning constructor, will try reusing buffer if the new window can fit in 
  BufferWindow(const BufferWindow& other, int left, int right, int top, int bottom);

  int GetPitch()   const { return vfb->GetPitch(); }
  int GetRowSize() const { return row_size; }
  int GetAlignedRowSize() const { int r = vfb->Aligned(row_size); return r <= vfb->GetPitch()? r : row_size; }
  int GetHeight()  const { return height; }
  int GetAlign() const { return vfb->GetAlign(); }
  
  const BYTE * GetReadPtr() const { return vfb->GetReadPtr() + offset; }
  BYTE * GetWritePtr();  

  bool IsInWindow(int x, int y) const { return x >= 0 && x < row_size && y >= 0 && y < height; }
  int VectorToWindowOffset(int x, int y) const { return y * GetPitch() + x; }

  const BYTE * GetReadPtr(int x, int y) const { return IsInWindow(x, y) ? GetReadPtr() + VectorToWindowOffset(x, y) : NULL; }
  BYTE * GetWritePtr(int x, int y) { return IsInWindow(x, y) ? GetWritePtr() + VectorToWindowOffset(x, y) : NULL; }

  //copy other into self at the given coords (which can be negatives)
  //only overlap is copied, no effect if there is none
  void Copy(const BufferWindow& other, int x, int y); 
  
  void Blend(const BufferWindow& other, float factor);

  void FlipVertical() {
    BufferWindow result(row_size, height, -vfb->GetAlign());
    IScriptEnvironment::BitBlt(result.GetWritePtr(), result.GetPitch(), 
      GetReadPtr() + (height - 1)*GetPitch(), -GetPitch(), row_size, height);
    *this = result;
  }

};



#endif //#ifndef __BUFFERWINDOW_H__