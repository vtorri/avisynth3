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

	AddRef();               //update refcount counting the new ref in ppv

	return S_OK;
}


//IAVIStream

STDMETHODIMP_(LONG) AviStream::Info(AVISTREAMINFOW * psi, LONG lSize)
{
  if ( lSize < (long)sizeof(AVISTREAMINFOW) )
    return AVIERR_BUFFERTOOSMALL; 
 
  if ( psi != NULL )
    FillAviStreamInfo(reinterpret_cast<AviStreamInfo *>(psi));

  return S_OK;
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



} } //namespace avs::vfw
