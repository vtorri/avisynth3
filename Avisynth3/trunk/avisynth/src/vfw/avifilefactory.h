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


#ifndef __AVS_AVIFILEFACTORY_H__
#define __AVS_AVIFILEFACTORY_H__

//avisynth include
#include "../com/base.h"


namespace avs { namespace vfw {



class AviFileFactory : public IClassFactory
                     , public com::Base
{

public:  //structors

  AviFileFactory() { }
  virtual ~AviFileFactory() { }


public:  //IUnknown

  STDMETHODIMP_(ULONG) AddRef() { return AddRef_(); }
  STDMETHODIMP_(ULONG) Release() { return Release_(); }
  STDMETHODIMP QueryInterface(IID const& iid, void **ppv);


public:  //IClassFactory

  STDMETHODIMP CreateInstance (LPUNKNOWN pUnkOuter, REFIID riid,  void ** ppvObj);
  STDMETHODIMP LockServer (BOOL fLock) { return S_OK; }


public:  //factory method

  static HRESULT Create(IID const& riid, void ** ppv);

};



} } //namespace avs::vfw

#endif //__AVS_AVIFILEFACTORY_H__