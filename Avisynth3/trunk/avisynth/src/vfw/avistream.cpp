// Avisynth v3.0 alpha.  Copyright 2004 David Pierre - Ben Rudiak-Gould et al.
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


//avisynth includes
#include "avifile.h"
#include "avistream.h"
#include "../core/clip.h"
#include "../core/videoinfo.h"
#include "../core/colorspace.h"
#include "../core/exception/generic.h"

//boost include
#include <boost/format.hpp>


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

#ifdef _MSC_VER
  EXCEPTION_POINTERS* ei = NULL;    
  DWORD code = 0;          //both init to avoid warning about may be not initialized

  __try { Read(lpBuffer, lStart, lSamples); }
  __except ( ei = GetExceptionInformation(), code = GetExceptionCode(), (code >> 28) == 0xC) 
  {
    switch (code) 
    {
    case EXCEPTION_ACCESS_VIOLATION:    ThrowAccessViolation(ei);
    case EXCEPTION_ILLEGAL_INSTRUCTION: ThrowIllegalInstruction(ei);
    case EXCEPTION_INT_DIVIDE_BY_ZERO:  ThrowIntDivideByZero(ei);      
    case EXCEPTION_STACK_OVERFLOW:      ThrowStackOverFlow();
    default:                            ThrowUnknownException(ei, code);
    }        
  }
#else
  Read(lpBuffer, lStart, lSamples);
#endif
}

void AviStream::ThrowAccessViolation(EXCEPTION_POINTERS * ei)
{
  throw exception::Generic(str( 
      boost::format("Avisynth: caught an access violation at 0x%08x,\nattempting to %s 0x%08x")
        % ei->ExceptionRecord->ExceptionAddress
        % ( ei->ExceptionRecord->ExceptionInformation[0] ? "write to" : "read from" )
        % ei->ExceptionRecord->ExceptionInformation[1]  
      ));
}

void AviStream::ThrowIllegalInstruction(EXCEPTION_POINTERS * ei)
{
  throw exception::Generic(str( boost::format("Avisynth: illegal instruction at 0x%08x") % ei->ExceptionRecord->ExceptionAddress ));
}

void AviStream::ThrowIntDivideByZero(EXCEPTION_POINTERS * ei)
{
  throw exception::Generic(str( boost::format("Avisynth: division by zero at 0x%08x") % ei->ExceptionRecord->ExceptionAddress ));
}

void AviStream::ThrowStackOverFlow()
{
  throw exception::Generic("Avisynth: stack overflow");
}

void AviStream::ThrowUnknownException(EXCEPTION_POINTERS * ei, DWORD code)
{
  throw exception::Generic(str( 
      boost::format("Avisynth: unknown exception 0x%08x at 0x%08x")
        % code 
        % ei->ExceptionRecord->ExceptionAddress
      ));
}


} } //namespace avs::vfw
