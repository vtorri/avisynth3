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
#include <utility>   //for pair
#include <boost/shared_ptr.hpp>





class VideoFrameBuffer : public RefCounted {

protected:
  typedef boost::shared_ptr<BYTE> Buffer;
  
  struct MemoryPiece {

    typedef vector<MemoryPiece> AllocationVector;

    static AllocationVector alloc;

    int size;
    Buffer data;

    MemoryPiece(int _size);
    MemoryPiece(const MemoryPiece& other) : size(other.size), data(other.data) { }

    static Save(const MemoryPiece & piece);
  };

  static int SizeFor(int pitch, int height) { return pitch * height + FRAME_ALIGN; }

public:
  virtual const BYTE * GetReadPtr() const = 0;
  //returns NULL when you are not allowed to write 
  virtual BYTE * GetWritePtr() = 0;

  virtual int GetSize() const = 0;
  virtual int GetPitch() const = 0;


  static int Aligned(int row_size, int align = FRAME_ALIGN) { return (row_size + (align - 1)) / align * align; }
};


typedef smart_ptr<VideoFrameBuffer> PVideoFrameBuffer;  




//frame buffer used to have multiple planes in one block
//used when inputting from vfw
class SolidFrameBuffer : public VideoFrameBuffer {

  MemoryPiece buffer;

public:
  SolidFrameBuffer(int size) : buffer(size) { }

  virtual const BYTE * GetReadPtr() const { return buffer.data.get(); }
  virtual BYTE * GetWritePtr() { return buffer.data.get(); }

  virtual int GetSize() const { return buffer.size; }
  virtual int GetPitch() const { throw std::logic_error(""); }

  ~SolidFrameBuffer() { MemoryPiece::Save(buffer); }
};

//standard frame buffer who holds one plane
class PlaneFrameBuffer : public SolidFrameBuffer {

  const int pitch;

public:
  PlaneFrameBuffer(int row_size, int height)
    : SolidFrameBuffer(SizeFor(Aligned(row_size), height)), pitch(Aligned(row_size)) { }

  virtual int GetPitch() const { return pitch; }
};



class ForwardingBuffer : public VideoFrameBuffer {

protected:
  const int pitch;
  PVideoFrameBuffer forwardTo;

  ForwardingBuffer(int row_size, int align) : pitch(Aligned(row_size, align)) { }

public:
  ForwardingBuffer(int _pitch, PVideoFrameBuffer _forwardTo) : pitch(_pitch), forwardTo(_forwardTo) { }

  virtual const BYTE * GetReadPtr() const { return forwardTo->GetReadPtr(); }
  virtual BYTE * GetWritePtr() { return forwardTo->GetWritePtr(); }

  virtual int GetPitch() const { return pitch; }
  virtual int GetSize() const { return forwardTo->GetSize(); }
};


//used in combo with SolidFrameBuffer
class PlaneForwardingBuffer : public ForwardingBuffer {

  int offset, size;

public:
  PlaneForwardingBuffer(int pitch, int _offset, int height, PVideoFrameBuffer forwardTo)
    : ForwardingBuffer(pitch, forwardTo), offset(_offset), size(pitch * height) { }

  virtual const BYTE * GetReadPtr() const { return forwardTo->GetReadPtr() + offset; }
  virtual BYTE * GetWritePtr() { return IsShared() ? NULL : forwardTo->GetWritePtr() + offset; }

  virtual int GetSize() const { return size; }

};



class AlphaForwardingBuffer : public ForwardingBuffer {
  
  //shared buffer for alpha planes
  class SharedAlphaBuffer : public SolidFrameBuffer {
  public:
    SharedAlphaBuffer(int size) : SolidFrameBuffer(size) { memset(GetWritePtr(), 255, GetSize()); }
  };

  static PVideoFrameBuffer largest;

public:
  AlphaForwardingBuffer(int row_size, int height);

  //never write into shared alpha buffers
  virtual BYTE * GetWritePtr() { return NULL; }

};






//BufferWindow holds a window in a possibly shared VideoFrameBuffer
//when asked for a write ptr, it will do a (intelligent) copy of the buffer if shared
class BufferWindow {

  PVideoFrameBuffer vfb;
  int offset, row_size, height;  

  static int FirstOffset(PVideoFrameBuffer vfb);

public:
  BufferWindow() { }
  BufferWindow(int _row_size, int _height) { NewWindow(_row_size, _height); }
  BufferWindow(const BufferWindow& other) : vfb(other.vfb), offset(other.offset), row_size(other.row_size), height(other.height) { }

  void NewWindow(int _row_size, int _height);
  void NewSolidWindow(int _row_size, int _height, int pitch, PVideoFrameBuffer solid, int solidOffset);
  void NewAlphaWindow(int _row_size, int _height);

  int GetPitch()   const { return vfb->GetPitch(); }
  int GetRowSize() const { return row_size; }
  int GetAlignedRowSize() const { int r = vfb->Aligned(row_size); return r <= vfb->GetPitch()? r : row_size; }
  int GetHeight()  const { return height; }
  
  const BYTE * GetReadPtr() const { return vfb->GetReadPtr() + offset; }
  BYTE * GetWritePtr();  

  //helper struct to simplify the code of Copy
  struct Vector {
    int x, y;
    Vector(int _x, int_y) : x(_x), y(_y) { }
  };

  bool IsInWindow(int x, int y) const { return x >= 0 && x < row_size && y >= 0 && y < height; }
  int VectorToWindowOffset(int x, int y) const { return y * GetPitch() + x; }

  Vector StickToWindow(int x, int y) const { return Vector( max(0, min(x, row_size)), max(0, min(y, height)) ); }

  const BYTE * GetReadPtr(int x, int y) const { return IsInWindow(x, y) ? GetReadPtr() + VectorToWindowOffset(x, y) : NULL; }
  BYTE * GetWritePtr(int x, int y) { return IsInWindow(x, y) ? GetWritePtr() + VectorToWindowOffset(x, y) : NULL; }

  //copy other into self at the given coords (which can be negatives)
  //only overlap is copied, no effect if there is none
  void Copy(const BufferWindow& other, int x, int y); 
  
  //smart crop (can upsize with negative params)
  //wil try to avoid buffer reallocation when possible
  void Crop(int left, int right, int top, int bottom);

  void Blend(const BufferWindow& other, float factor);

  void FlipVertical();

};



#endif //#ifndef __BUFFERWINDOW_H__