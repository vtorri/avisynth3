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


#ifndef __AVS_VFW_AVISFILE_H__
#define __AVS_VFW_AVISFILE_H__

//avisynth include
#include "../com/base.h"
#include "../core/smart_ptr_fwd.h"   //for PClip, CPVideoInfo

//stl include
#include <string>

//windows include
#include <vfw.h>


namespace avs { namespace vfw {


//declaration
class AviStream;



class AviFile : public IAVIFile
              , public IPersistFile 
              //, public IAvisynthClipInfo
              , public com::Base
{
    
  PClip clip_;
  CPVideoInfo vi_;
  std::string scriptName_;
    
  std::string error_msg_;


public:  //structors

  AviFile() { }
  virtual ~AviFile() { }


public:  //factory method

	static HRESULT Create(IID const& riid, void ** ppv);


public:  //IUnknown

  STDMETHODIMP_(ULONG) AddRef() { return AddRef_(); }
  STDMETHODIMP_(ULONG) Release() { return Release_(); }
	STDMETHODIMP QueryInterface(const IID& iid, void **ppv);


public:  //IPersist

	STDMETHODIMP GetClassID(LPCLSID lpClassID);


public:  //IPersistFile

  STDMETHODIMP IsDirty() { 	return S_FALSE; }
	STDMETHODIMP Load(LPCOLESTR lpszFileName, DWORD grfMode);
  STDMETHODIMP Save(LPCOLESTR lpszFileName, BOOL fRemember) { return E_FAIL; }
  STDMETHODIMP SaveCompleted(LPCOLESTR lpszFileName) { return S_OK; }
  STDMETHODIMP GetCurFile(LPOLESTR *lplpszFileName) { *lplpszFileName = NULL; return E_FAIL; }

	
public:  //IAVIFile

  STDMETHODIMP CreateStream(PAVISTREAM *ppStream, AVISTREAMINFOW *psi) { *ppStream = NULL; return AVIERR_READONLY; }
  STDMETHODIMP EndRecord() { return AVIERR_READONLY; }
	STDMETHODIMP GetStream(PAVISTREAM *ppStream, DWORD fccType, LONG lParam);
	STDMETHODIMP Info(AVIFILEINFOW *psi, LONG lSize);
	//STDMETHODIMP Open(LPCSTR szFile, UINT mode, LPCOLESTR lpszFileName);
  STDMETHODIMP ReadData(DWORD fcc, LPVOID lp, LONG *lpcb) { return AVIERR_NODATA; }
  STDMETHODIMP WriteData(DWORD fcc, LPVOID lpBuffer, LONG cbBuffer) { return AVIERR_READONLY; }
  STDMETHODIMP DeleteStream(DWORD fccType, LONG lParam) { return AVIERR_READONLY; }

  //STDMETHODIMP Save(LPCSTR szFile, AVICOMPRESSOPTIONS FAR *lpOptions, AVISAVECALLBACK lpfnCallback) { return AVIERR_READONLY; }
				

private:  //implementation
    
  bool DelayedInit();    
  void MakeErrorStream(std::string const& msg) { }

  friend class AviStream;  //so can call the above


  AviStream * CreateStream(DWORD fccType, int lParam);

};


} } //namespace avs::vfw

#endif //__AVS_VFW_AVISFILE_H__
