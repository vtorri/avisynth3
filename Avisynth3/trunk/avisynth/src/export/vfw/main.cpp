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
#include "base.h"                //for InstanceCounter
#include "main.h"
#include "avifilefactory.h"

using namespace avs::export_::vfw;


//methods required of a COM dll

BOOL APIENTRY DllMain(HANDLE /*hModule*/, ULONG ulReason, LPVOID /*lpReserved*/) 
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
  return rclsid == CLSID_CAVIFileSynth ? AviFileFactory::Create(riid, ppv)
                                       : CLASS_E_CLASSNOTAVAILABLE;
}


STDAPI DllCanUnloadNow() 
{
  return InstanceCounter::Count() == 0 ? S_OK : S_FALSE;
}



#endif //_WIN32
