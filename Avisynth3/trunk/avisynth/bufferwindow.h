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
#include "environment.h"
#include <vector>
#include <boost/shared_ptr.hpp>

#define FRAME_ALIGN 16 

typedef unsigned char BYTE;


class MemoryBuffer {

  struct MemoryPiece {

    typedef vector<MemoryPiece> AllocationVector;
    static AllocationVector recycle;

    boost::shared_ptr<BYTE> data;
    int size;

    MemoryPiece() : size(0) { }
    MemoryPiece(int _size, int& offset);
  };


  MemoryPiece buffer;
  PEnvironment env;

public:
  MemoryBuffer(int size, int& offset, ScriptEnvironment& _env)
    : buffer(size, offset), env(_env) { env->RegisterMemory(buffer.size); }

  ~MemoryBuffer() { MemoryPiece::recycle.push_back(buffer); env->FreeMemory(buffer.size); }

  int size() const { return buffer.size; }
  BYTE * get() { return buffer.data.get(); }
  const BYTE * get() const { return buffer.data.get(); }

  ScriptEnvironment& GetEnvironment() const { return *env; }
};





//exception class for trying to write on shared buffers
class shared_buffer : std::runtime_error {
public:
  shared_buffer() : std::runtime_error("Shared Buffer") { }
};

class ForwardingBuffer;

class VideoFrameBuffer : public RefCounted {

protected:
  virtual BYTE * UnProtectedGetWritePtr() = 0;

  friend class ForwardingBuffer;  //so it can use the above

public:
  virtual const BYTE * GetReadPtr() const = 0;  
  BYTE * GetWritePtr() throw(shared_buffer)
  {
    if ( IsShared() )
      throw shared_buffer();
    return UnProtectedGetWritePtr();
  }

  virtual int GetSize() const = 0;
  virtual int GetPitch() const = 0;

  virtual ScriptEnvironment& GetEnvironment() const = 0;
};


typedef smart_ptr<VideoFrameBuffer> PVideoFrameBuffer;  


//frame buffer who actually allocates the memory (not just forwarding)
class MemoryFrameBuffer : public VideoFrameBuffer {

  MemoryBuffer buffer;

protected:
  virtual BYTE * UnProtectedGetWritePtr() { return buffer.get(); }

public:
  MemoryFrameBuffer(int size, int& offset, ScriptEnvironment& env) : buffer(size, offset, env) { }

  virtual const BYTE * GetReadPtr() const { return buffer.get(); }

  virtual int GetSize() const { return buffer.size(); }
  virtual int GetPitch() const { throw std::logic_error("Illegal GetPitch Call"); }

  virtual ScriptEnvironment& GetEnvironment() const { return buffer.GetEnvironment(); }
};


//standard frame buffer who holds one plane
class PlaneFrameBuffer : public MemoryFrameBuffer {

  const int pitch;

public:
  PlaneFrameBuffer(int _pitch, int height, int& offset, ScriptEnvironment& env)
    : MemoryFrameBuffer(_pitch * height, offset, env), pitch(_pitch) { }

  virtual int GetPitch() const { return pitch; }
};



class ForwardingBuffer : public VideoFrameBuffer {

  int pitch;
  PVideoFrameBuffer forwardTo;

protected:
  virtual BYTE * UnProtectedGetWritePtr() { return forwardTo->UnProtectedGetWritePtr(); }

public:
  ForwardingBuffer(int _pitch, VideoFrameBuffer& _forwardTo) : pitch(_pitch), forwardTo(_forwardTo) { }

  virtual const BYTE * GetReadPtr() const { return forwardTo->GetReadPtr(); }

  virtual int GetPitch() const { return pitch; }

  virtual ScriptEnvironment& GetEnvironment() const { return forwardTo->GetEnvironment(); }
};


//used in combo with SolidFrameBuffer
class PlaneForwardingBuffer : public ForwardingBuffer {

  int offset, size;

  virtual BYTE * UnProtectedGetWritePtr() { return ForwardingBuffer::UnProtectedGetWritePtr() + offset; }

public:
  PlaneForwardingBuffer(int pitch, int _offset, int height, MemoryFrameBuffer& forwardTo)
    : ForwardingBuffer(pitch, forwardTo), offset(_offset), size(pitch * height) { }

  virtual const BYTE * GetReadPtr() const { return ForwardingBuffer::GetReadPtr() + offset; }

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

  static PlaneFrameBuffer * MakeFrameBuffer(const Dimension& dimension, int& offset, ScriptEnvironment& env) { return new PlaneFrameBuffer(Align(dimension.GetWidth()), dimension.GetHeight(), offset, env); }

  static int Limit(int value, int max) { return value < 0 ? 0 : value > max ? max : value; }

public:
  BufferWindow(const FrameVideoProperties& fvp, Plane plane, ScriptEnvironment& env)
    : dimension(fvp.GetPlaneDimension(plane)), vfb(MakeFrameBuffer(fvp.GetDimension(), offset, env)) { }
  BufferWindow(const Dimension& _dimension, ScriptEnvironment& env)
    : dimension(_dimension), vfb(MakeFrameBuffer(_dimension, offset, env)) { }
  BufferWindow(const BufferWindow& other) : dimension(other.dimension), vfb(other.vfb), offset(other.offset) { }


  const Dimension& GetDimension() const { return dimension; }
  int GetPitch() const { return vfb->GetPitch(); }

  int GetWidth() const { return dimension.GetWidth(); }
  int GetHeight() const { return dimension.GetHeight(); }
  
  const BYTE * GetReadPtr() const { return vfb->GetReadPtr() + offset; }
  BYTE * GetWritePtr();  

  ScriptEnvironment& GetEnvironment() const { return vfb->GetEnvironment(); }

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
  void FlipHorizontal(int grain = 1);

};



#endif //#ifndef __BUFFERWINDOW_H__