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
#include "main.h"
#include "com/base.h"
#include "vfw/factory.h"



extern "C" const GUID IID_IAvisynthClipInfo   // {E6D6B708-124D-11D4-86F3-DB80AFD98778}
  = {0xe6d6b708, 0x124d, 0x11d4, {0x86, 0xf3, 0xdb, 0x80, 0xaf, 0xd9, 0x87, 0x78}};



struct IAvisynthClipInfo : IUnknown {
  virtual int __stdcall GetError(const char** ppszMessage) = 0;
  virtual bool __stdcall GetParity(int n) = 0;
  virtual bool __stdcall IsFieldBased() = 0;
};





BOOL APIENTRY DllMain(HANDLE hModule, ULONG ulReason, LPVOID lpReserved) {

	switch(ulReason) {
	case DLL_PROCESS_ATTACH:
		CoInitialize(NULL);
		//_RPT0(0,"Process attach\n");
		break;

	case DLL_PROCESS_DETACH:
		CoUninitialize();
		//_RPT0(0,"Process detach\n");
		break;
	}

    return TRUE;
}


// From the Microsoft AVIFile docs.  Dense code...



STDAPI DllGetClassObject(CLSID const& rclsid, IID const& riid, void ** ppv)
{
  return rclsid == CLSID_CAVIFileSynth ? avs::AviFileFactory::Create(riid, ppv)
                                       : CLASS_E_CLASSNOTAVAILABLE;
}



STDAPI DllCanUnloadNow() 
{
  return avs::com::Base::ObjectCount() == 0 ? S_OK : S_FALSE;
}


