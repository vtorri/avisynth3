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

#include "videoproperties.h"  //chich includes geometric.h, colorspace.h, refcounted.h
#include <vector>
#include <boost/shared_ptr.hpp>

#define FRAME_ALIGN 16 

typedef unsigned char BYTE;






class VideoFrameBuffer : public RefCounted {

protected:
  
  struct MemoryPiece {

    typedef boost::shared_ptr<BYTE> Buffer;
    Buffer data;
    int size;

    MemoryPiece(int _size, int& offset);
    MemoryPiece(const MemoryPiece& other) : size(other.size), data(other.data) { }

    
    typedef vector<MemoryPiece> AllocationVector;
    static AllocationVector alloc;

    void Save() const { alloc.push_back(*this); }
  };


public:
  virtual const BYTE * GetReadPtr() const = 0;
  //returns NULL when you are not allowed to write 
  virtual BYTE * GetWritePtr() = 0;

  virtual int GetSize() const = 0;
  virtual int GetPitch() const = 0;

};


typedef smart_ptr<VideoFrameBuffer> PVideoFrameBuffer;  




//frame buffer used to have multiple planes in one block
//used when inputting from vfw
class SolidFrameBuffer : public VideoFrameBuffer {

  MemoryPiece buffer;

public:
  SolidFrameBuffer(int size, int& offset) : buffer(size, offset) { }

  virtual const BYTE * GetReadPtr() const { return buffer.data.get(); }
  virtual BYTE * GetWritePtr() { return buffer.data.get(); }

  virtual int GetSize() const { return buffer.size; }
  virtual int GetPitch() const { throw std::logic_error("Illegal GetPitch Call"); }

  ~SolidFrameBuffer() { buffer.Save(); }
};

//standard frame buffer who holds one plane
class PlaneFrameBuffer : public SolidFrameBuffer {

  const int pitch;

public:
  PlaneFrameBuffer(int _pitch, int height, int& offset)
    : SolidFrameBuffer(_pitch * height, offset), pitch(_pitch) { }

  virtual int GetPitch() const { return pitch; }
};



class ForwardingBuffer : public VideoFrameBuffer {

protected:
  const int pitch;
  PVideoFrameBuffer forwardTo;

//  ForwardingBuffer(int row_size, int align) : pitch(Aligned(row_size, align)) { }

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








//BufferWindow holds a window in a possibly shared VideoFrameBuffer
//when asked for a write ptr, it will do a (intelligent) copy of the buffer if shared
class BufferWindow {

  Dimension dimension;
  PVideoFrameBuffer vfb;
  int offset;  

  //this method expect align to be a power of 2 (FRAME_ALIGN is)
  static int Align(int dim, int align = FRAME_ALIGN) { return ( dim + align - 1 ) & -align; }

  static PlaneFrameBuffer * MakeFrameBuffer(const Dimension& dimension, int& offset) { return new PlaneFrameBuffer(Align(dimension.GetWidth()), dimension.GetHeight(), offset); }

  static int Limit(int value, int max) { return value < 0 ? 0 : value > max ? max : value; }

public:
  BufferWindow(const FrameVideoProperties& fvp, Plane plane)
    : dimension(fvp.GetPlaneDimension(plane)), vfb(MakeFrameBuffer(fvp.GetDimension(), offset)) { }
  BufferWindow(const Dimension& _dimension) : dimension(_dimension), vfb(MakeFrameBuffer(_dimension, offset)) { }
  BufferWindow(const BufferWindow& other) : dimension(other.dimension), vfb(other.vfb), offset(other.offset) { }


  const Dimension& GetDimension() const { return dimension; }
  int GetPitch() const { return vfb->GetPitch(); }

  int GetWidth() const { return dimension.GetWidth(); }
  int GetHeight() const { return dimension.GetHeight(); }
  
  const BYTE * GetReadPtr() const { return vfb->GetReadPtr() + offset; }
  BYTE * GetWritePtr();  

  Vecteur LimitToWindow(const Vecteur& vect) const { return Vecteur(Limit(vect.GetX(), GetWidth()), Limit(vect.GetY(), GetHeight())); }

  int VecteurToOffset(const Vecteur& vect) const { return vect.GetY() * GetPitch() + vect.GetX(); }

  const BYTE * GetReadPtr(const Vecteur& vect) const { return GetReadPtr() + VecteurToOffset(vect); }
  BYTE * GetWritePtr(const Vecteur& vect) { return GetWritePtr() + VecteurToOffset(vect); }

  //buffer resizing method
  //will try to avoid buffer reallocation when possible
  void SizeChange(const Vecteur& topLeft, const Vecteur& bottomRight);

  //copy other into self at the given coords (which can be negatives)
  //only overlap is copied, no effect if there is none
  void Copy(const BufferWindow& other, const Vecteur& coords); 
  

  void Blend(const BufferWindow& other, float factor);

  void FlipVertical();
  void FlipHorizontal(int grain);

};



#endif //#ifndef __BUFFERWINDOW_H__