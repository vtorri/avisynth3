// Avisynth v3.0 alpha.  Copyright 2004-2006 David Pierre - Ben Rudiak-Gould et al.
// http://www.avisynth.org

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; witho4ut even the implied warranty of
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
#include "../../define.h" //included by native.h anyway
#include "native.h"

//stlport include
#include <cassert>

#ifdef _WIN32
//windows includes
#include <windows.h>
#include <winbase.h>
#endif //_WIN32


namespace avs { namespace linker { namespace external {



void Native::FillFunctionList(FunctionList& addTo) const
{
  typedef PFunction (AVS_STDCALL * GetFunctionFunction)(int index);

  GetFunctionFunction gff = (GetFunctionFunction)GetProcedureAddress("GetFunction");
  assert( gff != NULL );

  for ( int i = 0; true; ++i )
  {
    PFunction funct = gff(i);
    if ( ! funct )
      break;
    addTo.push_back(funct);
  }

}


} } } //namespace avs::linker::external
