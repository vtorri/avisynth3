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


//avisynth include
#include "avifile.h"
#include "avistream.h"
#include "../core/clip.h"
#include "../core/videoinfo.h"
#include "../core/colorspace.h"


namespace avs { namespace vfw {



AviStream::AviStream(AviFile & parent)
  : parent_( parent )
{
  parent.AddRef();
}

AviStream::~AviStream()
{
  parent_.Release();
}



//IUnknown

STDMETHODIMP AviStream::QueryInterface(IID const& iid, void **ppv) 
{
	if ( iid == IID_IUnknown ) 
		*ppv = (IUnknown *)this;
	else 
    if ( iid == IID_IAVIStream )
		  *ppv = (IAVIStream *)this;
	  else 
    {
    	*ppv = NULL;
		  return E_NOINTERFACE;
	  }

	AddRef();

	return S_OK;
}


//IAVIStream

STDMETHODIMP_(LONG) AviStream::Info(AVISTREAMINFOW *psi, LONG lSize)
{
  if ( lSize < sizeof(AVISTREAMINFOW) )
    return AVIERR_BUFFERTOOSMALL; 

  memset(psi, 0, lSize);
  
  FillAviStreamInfo(*psi);

  return 0;
}

STDMETHODIMP_(LONG) AviStream::FindSample(LONG lPos, LONG lFlags)
{
  //NB: this implementation may be incomplete: what happens when lPos is not a valid sample

	if (lFlags & FIND_FORMAT)
		return -1;

	if (lFlags & FIND_FROM_START)
		return 0;

	return lPos;
}


PClip const& AviStream::GetClip()
{
  return parent_.clip_;
}

CPVideoInfo AviStream::GetVideoInfo()
{
  return parent_.vi_;
}

void AviStream::MakeErrorStream(std::string const& msg)
{
  parent_.MakeErrorStream(msg);
}


void AviStream::ReadWrapper(void* lpBuffer, int lStart, int lSamples)
{
  // It's illegal to call GetExceptionInformation() inside an __except
  // block!  Hence this variable and the horrible hack below...
#ifndef _DEBUG
  EXCEPTION_POINTERS* ei;
  DWORD code;
  __try { 
#endif

    Read(lpBuffer, lStart, lSamples);

#ifndef _DEBUG
  }
  __except (ei = GetExceptionInformation(), code = GetExceptionCode(), (code >> 28) == 0xC) {
    switch (code) {
    case EXCEPTION_ACCESS_VIOLATION:
      parent->env->ThrowError("Avisynth: caught an access violation at 0x%08x,\nattempting to %s 0x%08x",
        ei->ExceptionRecord->ExceptionAddress,
        ei->ExceptionRecord->ExceptionInformation[0] ? "write to" : "read from",
        ei->ExceptionRecord->ExceptionInformation[1]);
    case EXCEPTION_ILLEGAL_INSTRUCTION:
      parent->env->ThrowError("Avisynth: illegal instruction at 0x%08x",
        ei->ExceptionRecord->ExceptionAddress);
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
      parent->env->ThrowError("Avisynth: division by zero at 0x%08x",
        ei->ExceptionRecord->ExceptionAddress);
    case EXCEPTION_STACK_OVERFLOW:
      throw AvisynthError("Avisynth: stack overflow");
    default:
      parent->env->ThrowError("Avisynth: unknown exception 0x%08x at 0x%08x",
        code, ei->ExceptionRecord->ExceptionAddress);
    }
  }
#endif
}




} } //namespace avs::vfw