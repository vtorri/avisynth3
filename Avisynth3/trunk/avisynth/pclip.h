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


#ifndef __PCLIP_H__
#define __PCLIP_H__

// Win32 API macros, notably the types BYTE, DWORD, ULONG, etc. 
#include <windows.h>
#include <windef.h>  

#include "videoinfo.h"
#include "refcounted.h"

enum { AVISYNTH_INTERFACE_VERSION = 3 };

#define FRAME_ALIGN 16 
// Default frame alignment is 16 bytes, to help P4, when using SSE2
#pragma warning( disable : 4290 )



/********************************************************************************************
 * Polymorphic VideoFrame class                                                             *
 ********************************************************************************************
  VideoFrameBuffer no longer defined there (in videoframe.h)
  VideoFrame still use them, but they are hidden by the VideoFrame API

  MakeWritable is no longer required : CPVideoFrame are never Writable, nor mutable in any sort
  PVideoframe are always writable/mutable, and will copy buffers before returning Write Ptr if they are shared
*/

class VideoFrame;
typedef smart_ptr<VideoFrame> PVideoFrame;
typedef smart_ptr_to_cst<VideoFrame> CPVideoFrame;



class VideoFrame : public RefCounted {

public:

  //static method to convert index in Plane enum, USE it !!!
  //the NOT_PLANAR plane is provided in last position
  //for cases where you process all planes in the same way (use with ColorSpace::HasPlane(Plane) )
  inline static Plane IndexToPlane(int i) {
    static Plane planes[] = { PLANAR_Y, PLANAR_U, PLANAR_V, NOT_PLANAR };
    return planes[i];
  }


  //some legacy methods, no longer get plane Y when planar
  int GetPitch() const throw(NoSuchPlaneException) { return GetPitch(NOT_PLANAR); }
  int GetRowSize() const throw(NoSuchPlaneException) { return GetRowSize(NOT_PLANAR); }
  int GetAlignedRowSize() const throw(NoSuchPlaneException) { return GetAlignedRowSize(NOT_PLANAR); }
  int GetHeight() const throw(NoSuchPlaneException) { return GetHeight(NOT_PLANAR); }

  const BYTE * GetReadPtr() const throw(NoSuchPlaneException) { return GetReadPtr(NOT_PLANAR); }
  BYTE * GetWritePtr() throw(NoSuchPlaneException) { return GetWritePtr(NOT_PLANAR); }
  
  //new virtual ones
  virtual int GetPitch(Plane plane) const throw(NoSuchPlaneException) = 0;
  virtual int GetRowSize(Plane plane) const throw(NoSuchPlaneException) = 0;
  virtual int GetAlignedRowSize(Plane plane) const throw(NoSuchPlaneException) = 0;
  virtual int GetHeight(Plane plane) const throw(NoSuchPlaneException) = 0;

  virtual const BYTE * GetReadPtr(Plane plane) const throw(NoSuchPlaneException) = 0;
  virtual BYTE * GetWritePtr(Plane plane) throw(NoSuchPlaneException) = 0;
  
  //methods to get some other general infos
  virtual const ColorSpace& GetColorSpace() const = 0;
  bool IsPlanar() const { return GetColorSpace().HasProperty(ColorSpace::PLANAR); }  

  //method to get Video width and height
  virtual int GetVideoHeight() const = 0; 
  virtual int GetVideoWidth() const = 0; 

  virtual bool IsInterlaced() const = 0;

  //construction method, moved from IScriptEnvironment
  static PVideoFrame NewVideoFrame(const VideoInfo& vi, int align = FRAME_ALIGN);
  //same as the above but with inheritance of the non volatiles properties
//  virtual PVideoFrame NewVideoFrame(const VideoInfo& vi, int align = FRAME_ALIGN) const = 0; 


  /************************************************************************************
   *  various ToolBox methods now provided by the VideoFrame API                       *
   ************************************************************************************/

  //ColorSpace conversion method
  virtual CPVideoFrame ConvertTo(const ColorSpace& space) const = 0;

  //Please Note that those dimensions are in PIXELS, and can be negative
  //positive values crop, negatives increase size, buffers won't be reallocated if possible
  //if reallocation has to be done, original data is copied at the right place
  virtual void SizeChange(int left, int right, int top, int bottom) = 0;

  //Copy other into self, the coords left and top can be negative
  //Only the overlapping part is copied
  virtual void Copy(CPVideoFrame other, int left, int top) = 0;

  void StackHorizontal(CPVideoFrame other); //implemented using SizeChange and Copy 
  void StackVertical(CPVideoFrame other);   
  
  virtual void FlipVertical() = 0;
  virtual void FlipHorizontal() = 0;

  virtual void TurnLeft() = 0;  //how do we turn a YUY2 frame !?...
  virtual void TurnRight() = 0;

  virtual void Blend(CPVideoFrame other, float factor) = 0;

  //what else ???

  /************************************************************************************
   *  Custom Properties System                                                        *
   ************************************************************************************/

  virtual CPProperty GetProperty(PPropertyKey key) const = 0;
  virtual void SetProperty(CPProperty prop) = 0;
  virtual void RemoveProperty(PPropertyKey key) = 0;

};

//template specialisations, so properties will be get cleaned correctly
//TODO...





/********************************************************************************************
 * IClip and PClip                                                                          *
 ********************************************************************************************
  Base class for all filters.
*/

class IClip : public RefCounted {

public:

  enum CachePolicy {
    CACHE_NOTHING,
    CACHE_RANGE,
    CACHE_LAST
  };

  IClip() { }

  virtual int __stdcall GetVersion() { return AVISYNTH_INTERFACE_VERSION; }
  
  virtual CPVideoFrame __stdcall GetFrame(int n) = 0;
  virtual bool __stdcall GetParity(int n) = 0;  // return field parity if field_based, else parity of first field in frame
  virtual void __stdcall GetAudio(void* buf, __int64 start, __int64 count) = 0;  // start and count are in samples
  virtual void __stdcall SetCacheHints(CachePolicy policy, int size) { }  // We do not pass cache requests upwards, only to the next filter.
  virtual const VideoInfo& __stdcall GetVideoInfo() = 0;

};

typedef smart_ptr<IClip> PClip;  // smart pointer to IClip



// instanciable null filter that forwards all requests to child
// use for filter who don't change VideoInfo
class StableVideoFilter : public IClip {

protected:
  PClip child;

  //protected constructor
  StableVideoFilter(PClip _child) : child(_child) { }

public:
  CPVideoFrame __stdcall GetFrame(int n) { return child->GetFrame(n); }
  void __stdcall GetAudio(void* buf, __int64 start, __int64 count) { child->GetAudio(buf, start, count); }
  const VideoInfo& __stdcall GetVideoInfo() { return child->GetVideoInfo(); }
  bool __stdcall GetParity(int n) { return child->GetParity(n); }
};


// instance null filter
// use when VideoInfo is changed
class GenericVideoFilter : public StableVideoFilter {

protected:
  VideoInfo vi;

  //protected constructor
  GenericVideoFilter(PClip _child, const VideoInfo& _vi) : StableVideoFilter(_child), vi(_vi) { }

    
public:
  const VideoInfo& __stdcall GetVideoInfo() { return vi; }

};



#endif //#ifndef __PCLIP_H__