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


//avisynth includes
#include "../main.h"
#include "avifile.h" 
#include "avistream/yv12.h"
#include "avistream/audio.h"
#include "avistream/interleaved.h"
#include "../core/clip.h"
#include "../core/videoinfo.h"
#include "../core/colorspace.h"
#include "../core/exception/generic.h"
#include "../core/runtime_environment.h"
#include "../filters/source/staticimage.h"
#include "../parser/parser.h"

//stl include
#include <fstream>
#include <sstream>

//assert include
#include <assert.h>


extern "C" const GUID CLSID_CAVIFileSynth   // {E6D6B700-124D-11D4-86F3-DB80AFD98778}
  = {0xe6d6b700, 0x124d, 0x11d4, {0x86, 0xf3, 0xdb, 0x80, 0xaf, 0xd9, 0x87, 0x78}};


namespace avs { namespace vfw {



HRESULT AviFile::Create(IID const& riid, void **ppv)
{
	AviFile * pAviFile;

  try { pAviFile = new AviFile(); }
  catch(std::bad_alloc&)
  {
    *ppv = NULL;
    return E_OUTOFMEMORY;
  }

	HRESULT hresult = pAviFile->QueryInterface(riid, ppv);
	pAviFile->Release();

	return hresult;
}


//IUnknown

STDMETHODIMP AviFile::QueryInterface(IID const& iid, void **ppv)
{
	if ( iid == IID_IUnknown )
		*ppv = (IUnknown *)(IAVIFile *)this;
	else 
    if ( iid == IID_IPersist )
      *ppv = (IPersist *)this;
	  else 
      if ( iid == IID_IPersistFile )
		    *ppv = (IPersistFile *)this;
	    else 
        if ( iid == IID_IAVIFile )
		      *ppv = (IAVIFile *)this;
	      else 
        {
      		*ppv = NULL;
		      return E_NOINTERFACE;
	      }

	AddRef();

	return S_OK;
}


//IPersist

STDMETHODIMP AviFile::GetClassID(LPCLSID lpClassID)
{
  *lpClassID = CLSID_CAVIFileSynth;
	return S_OK;
}


//IPersistFile

STDMETHODIMP AviFile::Load(LPCOLESTR lpszFileName, DWORD grfMode)
{
	if ( grfMode & ( OF_CREATE | OF_WRITE ) )
    return E_FAIL;

	char buffer[MAX_PATH];
	WideCharToMultiByte(AreFileApisANSI() ? CP_ACP : CP_OEMCP, 0, lpszFileName, -1, buffer, sizeof buffer, NULL, NULL); 

  try { scriptName_ = buffer; }
  catch(std::bad_alloc&) { return AVIERR_MEMORY; }

  return S_OK;
}


bool AviFile::DelayedInit()
{
  if ( ! clip_ )
  {
    try 
    {
/*      // create a script environment and load the script into it
      env = CreateScriptEnvironment();
      if (!env) return false;
      AVSValue return_val = env->Invoke("Import", szScriptName);
      // store the script's return value (a video clip)
      if (return_val.IsClip())
        filter_graph = ConvertAudio::Create(return_val.AsClip(), SAMPLE_INT8|SAMPLE_INT16|SAMPLE_INT24|SAMPLE_INT32, SAMPLE_INT16);  // Ensure samples are int     [filter_graph = return_val.AsClip();]      
      else      
        throw AvisynthError("The script's return value was not a video clip");
      if (!filter_graph)
        throw AvisynthError("The returned video clip was nil (this is a bug)");  */


      std::ifstream src( scriptName_.c_str() );
      std::stringstream dst;

      while ( ! src.eof() )
        dst.put( src.get() );

      clip_ = parser::Parser()(dst.str());

      // get information about the clip
      vi_ = clip_->GetVideoInfo();
      if ( vi_->IsYV12() && ( vi_->GetWidth() & 3 ) )
        throw exception::Generic("Avisynth error: YV12 images for output must have a width divisible by 4 (use crop)!");
      if ( vi_->IsYUY2() && ( vi_->GetWidth() & 3 ) )
        throw exception::Generic("Avisynth error: YUY2 images for output must have a width divisible by 4 (use crop)!");      
    }    
    catch (Exception& ex)
    {
      error_msg_ = ex.msg();
      clip_ = filters::StaticImage::CreateMessageClip( error_msg_, RuntimeEnvironment::Create(10000000) );        
      vi_ = clip_->GetVideoInfo();
    }    
    catch (...) { assert(false); return false; }    
  } 

  return true;
}

/*
void AviFile::MakeErrorStream(const char* msg) {
  error_msg = msg;
  filter_graph = Create_MessageClip(msg, vi->width, vi->height, vi->pixel_type, false, 0xFF3333, 0, 0, env);
}*/


STDMETHODIMP AviFile::Info(AVIFILEINFOW *pfi, LONG lSize) 
{
  if ( lSize < sizeof(AVIFILEINFOW) )
    return AVIERR_BUFFERTOOSMALL;

	if ( ! DelayedInit() ) 
    return E_FAIL;

  memset(pfi, 0, sizeof(AVIFILEINFOW));

	int streamCount = 0;
	if ( vi_->HasVideo() )	++streamCount;
	if ( vi_->HasAudio() )	++streamCount;

	pfi->dwFlags				  = AVIFILEINFO_HASINDEX | AVIFILEINFO_ISINTERLEAVED;
	pfi->dwCaps					  = AVIFILECAPS_CANREAD | AVIFILECAPS_ALLKEYFRAMES | AVIFILECAPS_NOCOMPRESSION;	
	pfi->dwStreams				= streamCount;

  if ( vi_->HasVideo() )
  {
	  pfi->dwWidth				= vi_->GetWidth();
	  pfi->dwHeight				= vi_->GetHeight();
    pfi->dwRate					= vi_->GetFPSNumerator();
	  pfi->dwScale				= vi_->GetFPSDenominator();
    pfi->dwLength				= vi_->GetFrameCount();
  }
    
  wcscpy(pfi->szFileType, L"Avisynth");

	return S_OK;
}

STDMETHODIMP AviFile::GetStream(PAVISTREAM *ppStream, DWORD fccType, LONG lParam)
{
	if ( ! DelayedInit() ) 
    return E_FAIL;

  try { *ppStream = CreateStream(fccType, lParam); }
  catch(std::bad_alloc&) 
  { 
    *ppStream = NULL;
    return AVIERR_MEMORY; 
  }
  
  return ( *ppStream == NULL ) ? AVIERR_NODATA : 0;  //good return value
}



AviStream * AviFile::CreateStream(DWORD fccType, int lParam)
{
  if ( vi_->HasVideo() )
  {
    switch( lParam )
    {
    case 0: 
      if ( fccType == 0 || fccType == streamtypeVIDEO )
        switch( vi_->GetColorSpace().id() )
        {
        case ColorSpace::I_RGB24: return new avistream::Interleaved(*this, BI_RGB, 3);
        case ColorSpace::I_RGB32: return new avistream::Interleaved(*this, BI_RGB, 4);
        case ColorSpace::I_YUY2: return new avistream::Interleaved(*this, MAKEFOURCC('Y','U','Y','2'), 2);
        case ColorSpace::I_YV12: return new avistream::YV12(*this);
        }
      return NULL;
    case 1:
      if ( vi_->HasAudio() && ( fccType == 0 || fccType == streamtypeAUDIO ) )
        return new avistream::Audio(*this);
    }
  }
  else
  {
    if ( vi_->HasAudio() && lParam == 0 && ( fccType == 0 || fccType == streamtypeAUDIO ) )
      return new avistream::Audio(*this);
  }
    
  return NULL;
}
 


} } //namespace avs::vfw
