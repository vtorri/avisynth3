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
#include "native.h"
#include "../../core/exception/generic.h"

//boost includes
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#ifdef _WIN32
//windows includes
#include <windows.h>
#include <winbase.h>
#endif //_WIN32



namespace avs { namespace linker { namespace external {



Plugin::PluginFolder Plugin::pluginFolder;


Plugin::Plugin(std::string const& fileName)
  : fileName_( fileName )
{
#ifdef _WIN32
  HMODULE module = LoadLibrary(fileName_.c_str());
  if ( module == NULL )
#endif
    throw exception::Generic("Unable to load library");

  //TODO: add a plugin identity & version check here
}


Plugin::~Plugin()
{
#ifdef _WIN32
  HMODULE module = GetModuleHandle(fileName_.c_str());
  FreeLibrary(module);
#endif
}


char const * Plugin::GetName() const
{
  typedef char const * (__cdecl *GetNameFunction)();

  GetNameFunction gnf = (GetNameFunction)(GetProcAddress("GetName"));
  assert( gnf != NULL );

  return gnf();
}


PPlugin Plugin::Create(std::string const& fileName)
{
  std::string name = CompleteName(fileName);

  PExtPlugin result = pluginFolder[name];      //search for a folded version 
  if ( result )
    return result;                             //return it if found

  //else create, fold and return one
  return pluginFolder[name] = PExtPlugin( static_cast<Plugin *>(new Native(name)) );
}


void * Plugin::GetProcAddress(char const * procName) const
{
#ifdef _WIN32
  return (void *)::GetProcAddress(GetModuleHandle(fileName_.c_str()), procName);
#endif
}


bool Plugin::CanUnloadNow() const
{
  typedef int (__cdecl *CanUnloadNowFunction)();

  CanUnloadNowFunction cunf = (CanUnloadNowFunction)(GetProcAddress("CanUnloadNow"));
  assert( cunf != NULL );

  return cunf() != 0;
}


std::string Plugin::CompleteName(std::string const& fileName)
{
  return fileName;           //TODO: implement me non-trivially
}

} } } //namespace avs::linker::external
