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
#include "plugin.h"
#include "../../core/exception/generic.h"

//boost includes
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#ifdef _WIN32
//windows include
#include <windows.h>
#include <winbase.h>
#endif //_WIN32

namespace avs { namespace linker { namespace external {



Plugin::Plugin(std::string const& fileName)
  : fileName_( fileName )
{


}


/*
PPlugin Plugin::Create(std::string const& fileName)
{
  std::string name = CompleteName(fileName);

  PExtPlugin result = pluginFolder[name];      //search for a folded version 
  if ( result )
    return result;                             //return it if found

  HMODULE module = LoadLibrary(name.c_str());

  if ( module == NULL )
    throw exception::Generic("Unable to load library");


}*/

} } } //namespace avs::linker::external
