// Avisynth v3.0 alpha.  Copyright 2003-2006 David Pierre - Ben Rudiak-Gould et al.
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


#ifdef _WIN32


//avisynth includes
#include "avifile.h"
#include "avistream.h"
#include "avistreaminfo.h"


namespace avs { namespace export_ { namespace vfw {



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

	AddRef();               //update refcount counting the new ref in ppv

	return S_OK;
}


//IAVIStream

STDMETHODIMP_(LONG) AviStream::Info(AVISTREAMINFOW * psi, LONG lSize)
{
  if ( lSize < static_cast<long>(sizeof(AVISTREAMINFOW)) )
    return AVIERR_BUFFERTOOSMALL; 
 
  if ( psi != NULL )
    FillAviStreamInfo(psi);

  return S_OK;
}


STDMETHODIMP_(LONG) AviStream::FindSample(LONG lPos, LONG lFlags)
{
  switch ( lFlags & FIND_TYPE )
  {
  case FIND_FORMAT: return -1;
  case FIND_KEY:
    switch ( lFlags & FIND_DIR )
    {
    case FIND_PREV: return FindKeySample(lPos, true);
    case FIND_NEXT: return FindKeySample(lPos, false);
    //case FIND_FROM_START
    default: break;
    }
  //case FIND_ANY and falling from above switch
  default: return 0;
  }
}



} } } //namespace avs::export_::vfwm

#endif //_WIN32
