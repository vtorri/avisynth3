// Avisynth v3.0 alpha.  Copyright 2002 Ben Rudiak-Gould et al.
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
//
// As a special exception, the copyright holders of Avisynth give you
// permission to link Avisynth with independent modules that communicate with
// Avisynth solely through the interfaces defined in avisynth.h, regardless of the license
// terms of these independent modules, and to copy and distribute the
// resulting combined work under terms of your choice, provided that
// every copy of the combined work is accompanied by a complete copy of
// the source code of Avisynth (the version of Avisynth used to produce the
// combined work), being distributed under the terms of the GNU General
// Public License plus this exception.  An independent module is a module
// which is not derived from or based on Avisynth, such as 3rd-party filters,
// import and export plugins, or graphical user interfaces.




#ifndef __AVISYNTH_H__
#define __AVISYNTH_H__

enum { AVISYNTH_INTERFACE_VERSION = 3 };


/* Define all types necessary for interfacing with avisynth.dll
   Moved from internal.h */

// Win32 API macros, notably the types BYTE, DWORD, ULONG, etc. 
#include <windef.h>  

// COM interface macros
#include <objbase.h>

// Raster types used by VirtualDub & Avisynth
#define in64 (__int64)(unsigned short)
typedef unsigned long	Pixel;    // this will break on 64-bit machines!
typedef unsigned long	Pixel32;
typedef unsigned char Pixel8;
typedef long			PixCoord;
typedef	long			PixDim;
typedef	long			PixOffset;

//STL relatives include
#include <string>
using namespace std;

/* Compiler-specific crap */

// Tell MSVC to stop precompiling here
#ifdef _MSC_VER
  #pragma hdrstop
#endif

// Set up debugging macros for MS compilers; for others, step down to the
// standard <assert.h> interface
#ifdef _MSC_VER
  #include <crtdbg.h>
#else
  #define _RPT0(a,b) ((void)0)
  #define _RPT1(a,b,c) ((void)0)
  #define _RPT2(a,b,c,d) ((void)0)
  #define _RPT3(a,b,c,d,e) ((void)0)
  #define _RPT4(a,b,c,d,e,f) ((void)0)
  
  #define _ASSERTE(x) assert(x)
  #include <assert.h>
#endif



// I had problems with Premiere wanting 1-byte alignment for its structures,
// so I now set the Avisynth struct alignment explicitly here.
#pragma pack(push,8)

#define FRAME_ALIGN 16 
// Default frame alignment is 16 bytes, to help P4, when using SSE2



#include "videoinfo.h"
#include "refcounted.h"




enum {
  FILTER_TYPE=1,
  FILTER_INPUT_COLORSPACE=2,
  FILTER_OUTPUT_TYPE=9,
  FILTER_NAME=4,
  FILTER_AUTHOR=5,
  FILTER_VERSION=6,
  FILTER_ARGS=7,
  FILTER_ARGS_INFO=8,
  FILTER_ARGS_DESCRIPTION=10,
  FILTER_DESCRIPTION=11,
};
enum {  //SUBTYPES
  FILTER_TYPE_AUDIO=1,
  FILTER_TYPE_VIDEO=2,
  FILTER_OUTPUT_TYPE_SAME=3,
  FILTER_OUTPUT_TYPE_DIFFERENT=4,
};





/********************************************************************************************
 * Polymorphic VideoFrame class                                                             *
 ********************************************************************************************
  VideoFrameBuffer no longer defined in avisynth.h (videoframe.h)
  VideoFrame still use them, but they are hidden by the VideoFrame API
  due to architecture changes (3 buffers when planar), you must no longer assume pitchU = pitchV
  it won't happen often but it can
  MakeWritable is no longer required : CPVideoFrame are never Writable, nor mutable in any sort
  PVideoframe are always writable/mutable, and will copy buffers before returning Write Ptr if they are shared
*/

class VideoFrame;
typedef smart_ptr<VideoFrame> PVideoFrame;
typedef smart_ptr_to_cst<VideoFrame> CPVideoFrame;

enum Plane {
  NOT_PLANAR,
  PLANAR_Y,
  PLANAR_U,
  PLANAR_V
};

class VideoFrame : public RefCounted {

public:

  //some typedefs
  typedef short dimension;  
  typedef signed char Align;

  //static method to convert index in Plane enum, USE it !!!
  inline static Plane IndexToYUVPlane(int i) {
    static Plane planes[] = { PLANAR_Y, PLANAR_U, PLANAR_V };
    _ASSERTE( i>=0 && i < 3);
    return planes[i];
  }

  class NoSuchPlane { }; //exception for inadequate plane requests 

  //some legacy methods, no longer get plane Y when planar
  dimension GetPitch() const throw(NoSuchPlane) { return GetPitch(NOT_PLANAR); }
  dimension GetRowSize() const throw(NoSuchPlane) { return GetRowSize(NOT_PLANAR); }
  dimension GetAlignedRowSize() const throw(NoSuchPlane) { return GetAlignedRowSize(NOT_PLANAR); }
  dimension GetHeight() const throw(NoSuchPlane) { return GetHeight(NOT_PLANAR); }

  const BYTE * GetReadPtr() const throw(NoSuchPlane) { return GetReadPtr(NOT_PLANAR); }
  BYTE * GetWritePtr() throw(NoSuchPlane) { return GetWritePtr(NOT_PLANAR); }
  
  //new virtual ones
  virtual dimension GetPitch(Plane plane) const throw(NoSuchPlane) = 0;
  virtual dimension GetRowSize(Plane plane) const throw(NoSuchPlane) = 0;
  virtual dimension GetAlignedRowSize(Plane plane) const throw(NoSuchPlane) = 0;
  virtual dimension GetHeight(Plane plane) const throw(NoSuchPlane) = 0;

  virtual const BYTE * GetReadPtr(Plane plane) const throw(NoSuchPlane) = 0;
  virtual BYTE * GetWritePtr(Plane plane) throw(NoSuchPlane) = 0;
  
  //methods to get some other general infos
  virtual const ColorSpace& GetColorSpace() const = 0;
  bool IsPlanar() const { return GetColorSpace().HasFlag(ColorSpace::PLANAR); }  
  virtual Align GetAlign() const = 0;

  //frames now know about their fieldbased state
  bool IsFieldBased() const;
  void SetFieldBased(bool fieldBased);

  //method to get Video width and height, default implementation: not planar case
  virtual dimension GetVideoHeight() const = 0; 
  virtual dimension GetVideoWidth() const = 0; 

  //methods used to check that pixels args match Colorspace restrictions
  //they throw the appropriate error msg when arg are illegal
  //you are not supposed to use them, all calls are done internally where needed
  //btw, they don't check sign, just parity and such (sign check are done at a lower level)
  virtual dimension WidthToRowSize(dimension width) const = 0;
  virtual dimension HeightCheck(dimension height) const = 0;

  //construction method, moved from IScriptEnvironment
  static PVideoFrame NewVideoFrame(const VideoInfo& vi, Align align = FRAME_ALIGN);
  //same as the above but with inheritance of the non volatiles properties
  virtual PVideoFrame NewVideoFrame(const VideoInfo& vi, Align align = FRAME_ALIGN) const = 0; 


  /************************************************************************************
   *  various ToolBox methods now provided by the VideoFrame API                       *
   ************************************************************************************/

  //ColorSpace conversion method
  virtual CPVideoFrame ConvertTo(ColorSpace space) const = 0;

  //Please Note that those dimensions are in PIXELS, and can be negative
  //positive values crop, negatives increase size, buffers won't be reallocated if possible
  //if reallocation has to be done, original data is copied at the right place
  virtual void SizeChange(dimension left, dimension right, dimension top, dimension bottom) = 0;

  //Copy other into self, the coords left and top can be negative
  //Only the overlapping part is copied
  virtual void Copy(CPVideoFrame other, dimension left, dimension top) = 0;

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

  //class used as a name/type for properties
  //use like this: static PropertyName nameOfMyProperty;
  class PropertyName {
    char dummy;  //to ensure sizeof > 0, so consecutive instances won't fuck up
  public:
    PropertyName() { }
  };

  //Property class, you can subclass it to add parameters you need
  //subclass should implement an accurate operator == taking new parameters into account
  //if your subclass is not made of constant objects, reimplement clone too
  class Property : public RefCounted {
    const PropertyName * name;
    
  protected:
    Property(const Property& other) : name(other.name) { }
    //Property is constant here, so I can return safely this as a clone
    //REIMPLEMENT if your subclass is mutable
    virtual RefCounted * clone() const { return const_cast<Property*>(this); } 

  public:
    Property(const PropertyName& _name) : name(&_name) { }  

    bool IsType(const PropertyName& _name) const { return name == &_name; } //convenience version of IsType
    bool SameTypeAs(const Property& other) const { return name == other.name; }

    virtual bool IsVolatile() const { return false; }   //properties are sticky by default
    
    //virtual == operator, should be reimplemented to take new members into account if subclassed
    virtual bool operator==(const Property& other) const { return SameTypeAs(other); }
    //does not yield a total order when subclassing but since multiple instance of a subclass
    //are not supposed to cohabit it pose no problem.
    bool operator <(const Property& other) const { return name < other.name; }
  };
   
  typedef smart_ptr_to_cst<Property> CPProperty;  //smart const Property *
  typedef smart_ptr<Property> PProperty;  //you are not really supposed to use it, but you can

  virtual CPProperty GetProperty(const PropertyName& name) const = 0;
  virtual void SetProperty(CPProperty prop) = 0;
  void SetProperty(PProperty prop) { SetProperty(CPProperty(prop)); }  //exists so outside PProperty won't be casted to CPProperty by the call of the above and loses its ref
  virtual void RemoveProperty(const PropertyName& name) = 0;

protected:
  //cleanup of all volatiles properties
  virtual void CleanProperties() = 0;
  friend class smart_ptr<VideoFrame>; //so it can clean

};

//template specialisations, so properties will be get cleaned correctly
template <> smart_ptr<VideoFrame>::smart_ptr(smart_ptr_to_cst<VideoFrame>& other) { StealOrClone(other); if (ptr) ((VideoFrame*)ptr)->CleanProperties(); }
template <> smart_ptr<VideoFrame>::smart_ptr(const smart_ptr_to_cst<T>& other) { Clone(other); if (ptr) ((VideoFrame*)ptr)->CleanProperties(); }

template <> const smart_ptr<VideoFrame>& smart_ptr<VideoFrame>::operator =(smart_ptr_to_cst<VideoFrame>& other) { StealOrClone(other); if (ptr) ((VideoFrame*)ptr)->CleanProperties(); return *this; }
template <> const smart_ptr<VideoFrame>& smart_ptr<VideoFrame>::operator =(const smart_ptr_to_cst<VideoFrame>& other) { Clone(other);  if (ptr) ((VideoFrame*)ptr)->CleanProperties(); return *this; }


class IScriptEnvironment;

// Base class for all filters.
class IClip : public RefCounted {

public:

  enum CachePolicy {
    CACHE_NOTHING,
    CACHE_RANGE,
    CACHE_LAST
  };

  IClip() {}

  virtual int __stdcall GetVersion() { return AVISYNTH_INTERFACE_VERSION; }
  
  virtual CPVideoFrame __stdcall GetFrame(int n) = 0;
  virtual bool __stdcall GetParity(int n) = 0;  // return field parity if field_based, else parity of first field in frame
  virtual void __stdcall GetAudio(void* buf, __int64 start, __int64 count) = 0;  // start and count are in samples
  virtual void __stdcall SetCacheHints(CachePolicy policy, int size) { }  // We do not pass cache requests upwards, only to the next filter.
  virtual const VideoInfo& __stdcall GetVideoInfo() = 0;

};

typedef smart_ptr<IClip> PClip;  // smart pointer to IClip




enum AVSType {
  VOID_T,
  CLIP_T,
  BOOL_T,
  INT_T,
  //LONG_T,
  FLOAT_T,
  STRING_T,
  ARRAY_T
};

/*template <class T> class avs { static AVSType type; };

AVSType avs<void>::type  = VOID_T;
AVSType avs<PClip>::type = CLIP_T;
AVSType avs<bool>::type  = BOOL_T;
AVSType avs<int>::type   = INT_T;
//AVSType avs<__int64>::type = LONG_T;
AVSType avs<float>::type = FLOAT_T;
AVSType avs<string>::type = STRING_T;  */

class AVSValue {

  AVSType type;
  bool defined;

  void * data;
  
  void * buildValue() {
    switch(type)
    {      
      case CLIP_T:   return new PClip();
      case INT_T:    return new int;
      case BOOL_T:   return new bool;
      case FLOAT_T:  return new float;
      case STRING_T: return new string();
    }
    return NULL;  
  }
  void build() { data = buildValue(); }

  void destroy()
  {
    switch(type)
    {
      case CLIP_T:   delete (PClip *)data; break;
      case INT_T:    delete (int *)data; break;
      case BOOL_T:   delete (bool *)data; break;
      case FLOAT_T:  delete (float *)data; break;
      case STRING_T: delete (string *)data; break;
    }
  }

  void Assign(const AVSValue& other) {
    destroy();
    type = other.type;
    build();
    switch(type)
    {
      case CLIP_T:   *((PClip *)data) = *((PClip *)other.data); break;
      case INT_T:    *((int *)data) = *((int *)other.data); break;
      case BOOL_T:   *((bool *)data) = *((bool *)other.data); break;
      case FLOAT_T:  *((float *)data) = *((float *)other.data); break;
      case STRING_T: *((string *)data) = *((string *)other.data); break;
    }    
  }

  //three methods repeatly used in operators
  inline TypeCheck(AVSType expected) const {
    if (expected != type)
      throw InternalError("improper use of an AVSValue");
  }
  inline BeforeAffectCheck(AVSType expected)
  {
    TypeCheck(expected);
    defined = true;
  }
  inline BeforeCastCheck(AVSType expected) const
  {
    TypeCheck(expected);
    if (! defined)
      throw AvisynthError("use of an AVSValue who was not initialised");
  }

public:

  AVSValue() : defined(false), type(VOID_T) { }
  AVSValue(const AVSValue& other) : type(VOID_T) { Assign(other); }

  ~AVSValue() { destroy(); }

  //constructor by type, used by the parser
  AVSValue(AVSType _type) : defined(false), type(_type) { build(); }

  AVSValue(PClip clip)      : defined(true), type(CLIP_T)   { build(); *((PClip *)data) = clip; }
  AVSValue(int b)          : defined(true), type(BOOL_T)   { build(); *((int *)data) = b; }
  AVSValue(bool i)           : defined(true), type(INT_T)    { build(); *((bool *)data) = i; }
  AVSValue(float f)         : defined(true), type(FLOAT_T)  { build(); *((float *)data) = f; }
  AVSValue(const string& s) : defined(true), type(STRING_T) { build(); *((string *)data) = s; }


  const AVSValue& operator=(const AVSValue& other) { Assign(other); return *this; }

  // Note that we transparently allow 'int' to be treated as 'float'.
  // There are no int<->bool conversions, though..

  const AVSValue& operator=(PClip value)  { BeforeAffectCheck(CLIP_T);   *((PClip *)data)  = value; return *this; }
  const AVSValue& operator=(int value)    { 
    if (type == FLOAT_T) 
      return operator=( float(value) ); //allows float avsvalues to receive int values
    BeforeAffectCheck(INT_T);    *((int *)data)    = value; return *this;
  }
  const AVSValue& operator=(bool value)   { BeforeAffectCheck(BOOL_T);   *((bool *)data)   = value; return *this; }
  const AVSValue& operator=(float value)  { BeforeAffectCheck(FLOAT_T);  *((float *)data)  = value; return *this; }
  const AVSValue& operator=(string value) { BeforeAffectCheck(STRING_T); *((string *)data) = value; return *this; }

  operator PClip() const  { BeforeCastCheck(CLIP_T); return *((PClip *)data); }
  operator int()   const  { BeforeCastCheck(INT_T);  return *((int *)data); }
  operator bool()  const  { BeforeCastCheck(BOOL_T); return *((bool *)data); }
  operator float() const  { 
    if (type == INT_T)
      return operator int();  //alows int avsvalues to be treated as float
    BeforeCastCheck(FLOAT_T); return *((float *)data);
  }
  operator string() const { BeforeCastCheck(STRING_T); return *((string *)data); }

  //may come in handy in some overload resolution
  operator double() const { return operator float(); }

  //test if initialised
  bool Defined() const { return defined; }
  AVSType GetType() const { return type; }

  bool Is(AVSType _type) const { return type == _type; }

  bool IsClip()   const { return Is(CLIP_T); }
  bool IsBool()   const { return Is(BOOL_T); }
  bool IsInt()    const { return Is(INT_T); }
  bool IsFloat()  const { return Is(FLOAT_T) || Is(INT_T); }
  bool IsString() const { return Is(STRING_T); }

};

typedef vector<AVSValue> ArgVector;


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








/* Helper classes useful to plugin authors */


class ConvertAudio : public GenericVideoFilter 
/**
  * Helper class to convert audio to any format
 **/
{
public:
  ConvertAudio(PClip _clip, int prefered_format);
  void __stdcall GetAudio(void* buf, __int64 start, __int64 count);

  static PClip Create(PClip clip, int sample_type, int prefered_type);
  static AVSValue __cdecl Create_float(AVSValue args, void*);
  static AVSValue __cdecl Create_32bit(AVSValue args, void*);
  static AVSValue __cdecl Create_16bit(AVSValue args, void*);
  static AVSValue __cdecl Create_8bit(AVSValue args, void*);
  virtual ~ConvertAudio()
  {if (tempbuffer_size) {delete[] tempbuffer;tempbuffer_size=0;}}
private:
  void convertToFloat(char* inbuf, float* outbuf, char sample_type, int count);
  void convertFromFloat(float* inbuf, void* outbuf, char sample_type, int count);

  __inline int Saturate_int8(float n);
  __inline short Saturate_int16(float n);
  __inline int Saturate_int24(float n);
  __inline int Saturate_int32(float n);

  char src_format;
  char dst_format;
  int src_bps;
  char *tempbuffer;
  SFLOAT *floatbuffer;
  int tempbuffer_size;
};


// For GetCPUFlags.  These are backwards-compatible with those in VirtualDub.
enum {                    
                    /* slowest CPU to support extension */
  CPUF_FORCE			  = 0x01,   // N/A
  CPUF_FPU			    = 0x02,   // 386/486DX
  CPUF_MMX			    = 0x04,   // P55C, K6, PII
  CPUF_INTEGER_SSE	= 0x08,		// PIII, Athlon
  CPUF_SSE			    = 0x10,		// PIII, Athlon XP/MP
  CPUF_SSE2			    = 0x20,		// PIV, Hammer
  CPUF_3DNOW			  = 0x40,   // K6-2
  CPUF_3DNOW_EXT		= 0x80,		// Athlon
  CPUF_X86_64       = 0xA0,   // Hammer (note: equiv. to 3DNow + SSE2, which only Hammer
                              //         will have anyway)
};
#define MAX_INT 0x7fffffff
#define MIN_INT 0x80000000



class IScriptEnvironment {
public:
  virtual __stdcall ~IScriptEnvironment() {}

  virtual /*static*/ long __stdcall GetCPUFlags() = 0;


  __declspec(noreturn) void __stdcall ThrowError(const string& err_msg) { throw AvisynthError(err_msg); }

  class NotFound /*exception*/ {};  // thrown by Invoke and GetVar

  typedef AVSValue (__cdecl *ApplyFunc)(AVSValue args, void* user_data, IScriptEnvironment* env);

  virtual void __stdcall AddFunction(const char* name, const char* params, ApplyFunc apply, void* user_data) = 0;
  virtual bool __stdcall FunctionExists(const char* name) = 0;
  virtual AVSValue __stdcall Invoke(const char* name, const AVSValue args, const char** arg_names=0) = 0;

  virtual AVSValue __stdcall GetVar(const char* name) = 0;
  virtual bool __stdcall SetVar(const char* name, const AVSValue& val) = 0;
  virtual bool __stdcall SetGlobalVar(const char* name, const AVSValue& val) = 0;

  virtual void __stdcall PushContext(int level=0) = 0;
  virtual void __stdcall PopContext() = 0;


  static void __stdcall BitBlt(BYTE* dstp, int dst_pitch, const BYTE* srcp, int src_pitch, int row_size, int height);

  typedef void (__cdecl *ShutdownFunc)(void* user_data, IScriptEnvironment* env);
  virtual void __stdcall AtExit(ShutdownFunc function, void* user_data) = 0;

  virtual void __stdcall CheckVersion(int version = AVISYNTH_INTERFACE_VERSION) = 0;

	virtual int __stdcall SetMemoryMax(int mem) = 0;

  virtual int __stdcall SetWorkingDir(const char * newdir) = 0;

};


// avisynth.dll exports this; it's a way to use it as a library, without
// writing an AVS script or without going through AVIFile.
IScriptEnvironment* __stdcall CreateScriptEnvironment(int version = AVISYNTH_INTERFACE_VERSION);


#pragma pack(pop)

#endif //__AVISYNTH_H__
