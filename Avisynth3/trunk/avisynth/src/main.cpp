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
#include "main.h"

#ifdef _WIN32

#include "com/base.h"
#include "vfw/avifilefactory.h"



//methods required of a COM dll

BOOL APIENTRY DllMain(HANDLE hModule, ULONG ulReason, LPVOID lpReserved) 
{
	switch(ulReason) 
  {
	case DLL_PROCESS_ATTACH:
		CoInitialize(NULL);
		break;

	case DLL_PROCESS_DETACH:
		CoUninitialize();
		break;
	}

  return TRUE;
}


STDAPI DllGetClassObject(CLSID const& rclsid, IID const& riid, void ** ppv)
{
  return rclsid == CLSID_CAVIFileSynth ? avs::vfw::AviFileFactory::Create(riid, ppv)
                                       : CLASS_E_CLASSNOTAVAILABLE;
}


STDAPI DllCanUnloadNow() 
{
  return avs::com::Base::ObjectCount() == 0 ? S_OK : S_FALSE;
}


#endif //_WIN32
