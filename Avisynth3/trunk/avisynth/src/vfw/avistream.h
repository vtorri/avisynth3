// Avisynth v3.0 alpha.  Copyright 2004 Ben Rudiak-Gould et al.
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


#ifndef __AVS_VFW_AVISTREAM_H__
#define __AVS_VFW_AVISTREAM_H__

//avisynth include
#include "../com/base.h"
#include "../core/forward.h"

//stl include
#include <string>

//windows include
#include <vfw.h>


//declarations
namespace avs { namespace vfw { class AviFile; } }


namespace avs { namespace vfw {
  


class AviStream : public IAVIStream
                , public com::Base
{

	AviFile & parent_;


public:  //structors

	AviStream(AviFile& parent);
	virtual ~AviStream();


public:  //IUnknown

  STDMETHODIMP_(ULONG) AddRef() { return AddRef_(); }
  STDMETHODIMP_(ULONG) Release() { return Release_(); }
	STDMETHODIMP QueryInterface(IID const& iid, void **ppv);


public:  //IAVIStream

  STDMETHODIMP Create(LONG lParam1, LONG lParam2) { return AVIERR_READONLY; }
  STDMETHODIMP Delete(LONG lStart, LONG lSamples) { return AVIERR_READONLY; }
	STDMETHODIMP_(LONG) Info(AVISTREAMINFOW *psi, LONG lSize);
	STDMETHODIMP_(LONG) FindSample(LONG lPos, LONG lFlags);
  STDMETHODIMP ReadData(DWORD fcc, LPVOID lp, LONG *lpcb) { return AVIERR_NODATA; }
	STDMETHODIMP SetFormat(LONG lPos, LPVOID lpFormat, LONG cbFormat) { return AVIERR_READONLY; }
	STDMETHODIMP Write(LONG lStart, LONG lSamples, LPVOID lpBuffer, LONG cbBuffer, 
		DWORD dwFlags, LONG *plSampWritten, LONG *plBytesWritten) { return AVIERR_READONLY; }
    
  STDMETHODIMP WriteData(DWORD fcc, LPVOID lpBuffer, LONG cbBuffer) { return AVIERR_READONLY; }
	STDMETHODIMP SetInfo(AVISTREAMINFOW *psi, LONG lSize) { return AVIERR_READONLY; }


protected:  //AviStream implementation

  PClip const& GetClip();
  CPVideoInfo GetVideoInfo();
  void MakeErrorStream(std::string const& msg);
	void ReadWrapper(void* lpBuffer, int lStart, int lSamples);

  virtual void FillAviStreamInfo(AVISTREAMINFOW& asi) = 0;
  virtual void Read(void* lpBuffer, int lStart, int lSamples) = 0;

  //helpers for ReadWrapper
  __declspec(noreturn) void ThrowAccessViolation(EXCEPTION_POINTERS * ei);
  __declspec(noreturn) void ThrowIllegalInstruction(EXCEPTION_POINTERS * ei);
  __declspec(noreturn) void ThrowIntDivideByZero(EXCEPTION_POINTERS * ei);
  __declspec(noreturn) void ThrowStackOverFlow();
  __declspec(noreturn) void ThrowUnknownException(EXCEPTION_POINTERS * ei, DWORD code);

};



} } //namespace avs::vfw

#endif //__AVS_VFW_AVISTREAM_H__
