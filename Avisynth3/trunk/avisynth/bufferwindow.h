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

#include "videoproperties.h"  //which includes geometric.h, colorspace.h, refcounted.h
#include "memorymanager.h"

#define FRAME_ALIGN 16 

//class declarations
class VideoFrameBuffer;

//typedefs
typedef smart_ref<VideoFrameBuffer> PVideoFrameBuffer;


//videoframebuffer base class
class VideoFrameBuffer : public RefCounted {

public:
  virtual bool IsWritable() const { return ! IsShared(); }

  virtual const BYTE * GetReadPtr() const = 0;  
  virtual BYTE * GetWritePtr() = 0;


  virtual int GetSize() const = 0;
  virtual int GetPitch() const = 0;

  virtual PEnvironment GetEnvironment() const = 0;
  virtual void SetEnvironment(PEnvironment env) = 0;

  //methods related to sharing frames between envs  
  virtual bool IsEnvSharingEnabled() const { return false; }
  virtual PVideoFrameBuffer ShareWith(PEnvironment env)    //return a buffer owned by env sharing the same data as self
  { 
    throw std::logic_error("Illegal ShareWith Call");      //default imp: error  env shring not enabled
  }
};


//buffer who forwards all requests to a child of a templated video frame buffer type
template <class Forward> class ForwardingBuffer : public VideoFrameBuffer {

protected:
  typedef smart_ref<Forward> PForwardToBuffer;

  PForwardToBuffer forwardTo;

public:
  ForwardingBuffer(PForwardToBuffer _forwardTo) : forwardTo(_forwardTo) { }

  virtual bool IsWritable() const { return ! IsShared() && ! forwardTo->IsWritable(); }

  virtual const BYTE * GetReadPtr() const { return forwardTo->GetReadPtr(); }
  virtual BYTE * GetWritePtr() { return forwardTo->GetWritePtr(); }

  virtual int GetSize() const { return forwardTo->GetSize(); }
  virtual int GetPitch() const { return forwardTo->GetPitch(); }

  virtual PEnvironment GetEnvironment() const { return forwardTo->GetEnvironment(); }
  virtual void SetEnvironment(PEnvironment env) { forwardTo->SetEnvironment(env); }
};







//frame buffer who actually allocates the memory (not just forwarding)
class MemoryFrameBuffer : public VideoFrameBuffer {

  OwnedMemoryPiece buffer;

public:
  MemoryFrameBuffer(int size, int& offset, PEnvironment env)
    : buffer(size + FRAME_ALIGN, env) { offset = (~int(buffer.get()) & ~FRAME_ALIGN) + 1; }

  virtual const BYTE * GetReadPtr() const { return buffer.get(); }
  virtual BYTE * GetWritePtr() { return buffer.get(); }

  virtual int GetSize() const { return buffer.size(); }
  virtual int GetPitch() const { throw std::logic_error("Illegal GetPitch Call"); }

  virtual PEnvironment GetEnvironment() const { return buffer.GetEnvironment(); }
  virtual void SetEnvironment(PEnvironment env) { buffer.SetEnvironment(env); }
};


//standard frame buffer who holds one plane
class PlaneFrameBuffer : public MemoryFrameBuffer {

  const int pitch;

public:
  PlaneFrameBuffer(int _pitch, int height, int& offset, PEnvironment env)
    : MemoryFrameBuffer(_pitch * height, offset, env), pitch(_pitch) { }

  virtual int GetPitch() const { return pitch; }
};






//used to make a buffer into a bigger memory block
//this is needed coz vfw expect a single block when exporting planar images
//after that we need to separate the block into a buffer for each plane
class PlaneForwardingBuffer : public ForwardingBuffer<MemoryFrameBuffer> {

  const int pitch, offset, size;

public:
  PlaneForwardingBuffer(PForwardToBuffer forwardTo, int _pitch, int _offset, int height)
    : ForwardingBuffer<MemoryFrameBuffer>(forwardTo), pitch(_pitch), offset(_offset), size(pitch * height) { }

  virtual bool IsWritable() const { return VideoFrameBuffer::IsWritable(); }   

  virtual const BYTE * GetReadPtr() const { return ForwardingBuffer<MemoryFrameBuffer>::GetReadPtr() + offset; }
  virtual BYTE * GetWritePtr() { return ForwardingBuffer<MemoryFrameBuffer>::GetWritePtr() + offset; }

  virtual int GetSize() const { return size; }
  virtual int GetPitch() const { return pitch; }
};



class EnvSharingBuffer : public ForwardingBuffer<VideoFrameBuffer> {

  typedef vector<PEnvironment> EnvVector;

  EnvVector sharingEnvs;

public:
  //forwardTo should not be shared
  EnvSharingBuffer(PVideoFrameBuffer forwardTo)
    : ForwardingBuffer<VideoFrameBuffer>(forwardTo) { /*_ASSERTE(! forwardTo->IsShared() );*/ }

  void AddSharingEnv(PEnvironment env) { if ( env != GetEnvironment() ) sharingEnvs.push_back(env); }
  void RemoveSharingEnv(PEnvironment env);
};


class ForwardToSharingBuffer : public ForwardingBuffer<EnvSharingBuffer> {

  PEnvironment env;

  ForwardToSharingBuffer(PForwardToBuffer forwardTo, PEnvironment _env)
    : ForwardingBuffer<EnvSharingBuffer>(forwardTo), env(_env) { forwardTo->AddSharingEnv(env); }

public:
  //encapsulates vfb into a EnvSharingBuffer 
  ForwardToSharingBuffer(PVideoFrameBuffer vfb)
    : ForwardingBuffer<EnvSharingBuffer>( new EnvSharingBuffer(vfb) ), env(vfb->GetEnvironment()) { }

  ~ForwardToSharingBuffer() { forwardTo->RemoveSharingEnv(env); }

  //we are in the only case where env sharing is enabled
  virtual bool IsEnvSharingEnabled() const { return true; }
  virtual PVideoFrameBuffer ShareWith(PEnvironment _env) { return env == _env ? PVideoFrameBuffer(this, true) : new ForwardToSharingBuffer(forwardTo, env); }

};



//BufferWindow holds a window in a possibly shared VideoFrameBuffer
//when asked for a write ptr, it will do a (intelligent) copy of the buffer if shared
class BufferWindow {

  Dimension dimension;               //dimension of the window
  mutable PVideoFrameBuffer vfb;     //buffer who holds the window
  mutable int offset;                //offset from buffer start to window start

  static int Limit(int value, int max) { return value < 0 ? 0 : value > max ? max : value; }

  //this method expect align to be a power of 2 (FRAME_ALIGN is)
  static int Align(int dim, int align = FRAME_ALIGN) { return ( dim + align - 1 ) & -align; }

  static PVideoFrameBuffer MakeFrameBuffer(const Dimension& dimension, int& offset, PEnvironment env)
  {
    return new PlaneFrameBuffer(Align(dimension.GetWidth()), dimension.GetHeight(), offset, env);
  }

  void MakeBufferWritable() const;

public:
  //normal constructor
  BufferWindow(const Dimension& _dimension, PEnvironment env) : dimension(_dimension), vfb(MakeFrameBuffer(dimension, offset, env)) { }
  //copy constructor
  BufferWindow(const BufferWindow& other) : dimension(other.dimension), vfb(other.vfb), offset(other.offset) { }
  //copy constructor with sharing to env
  BufferWindow(const BufferWindow& other, PEnvironment env);


  const Dimension& GetDimension() const { return dimension; }
  int GetWidth() const { return dimension.GetWidth(); }
  int GetHeight() const { return dimension.GetHeight(); }

  int GetPitch() const { return vfb->GetPitch(); }

  const BYTE * GetReadPtr() const { return vfb->GetReadPtr(); }
  BYTE * GetWritePtr() { MakeBufferWritable(); return vfb->GetWritePtr(); }

  PEnvironment GetEnvironment() const { return vfb->GetEnvironment(); }


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