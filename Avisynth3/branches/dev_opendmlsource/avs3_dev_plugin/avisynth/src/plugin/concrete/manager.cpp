// Avisynth v3.0 alpha.  Copyright 2003-2005 David Pierre - Ben Rudiak-Gould et al.
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
#include "handle.h"
#include "manager.h"
#include "../linker.h"


namespace avs { namespace plugin { namespace concrete {



void Manager::DeclareLinker(PLinker const& linker)
{
  linkerMap_[linker->GetStrategy()] = linker;
}

PLinker const& Manager::GetLinker(std::string strategy) const
{
  LinkerMap::const_iterator it = linkerMap_.find(strategy);
  if ( it != linkerMap_.end() )
    return *it;

  throw exception::NoSuchLinker(strategy);
}


PPluginDef Manager::LoadPlugin(std::string const& fileName)
{
  PluginDefMap::iterator pIt = pluginDefMap_.find(filename);
  if ( pIt != pluginDefMap_.end() )
    return *pIt;

  boost::shared_ptr<Handle const> handle( new Handle(fileName) );
  PPlugin plugin = GetLinker(handle->GetLinkStrategy())->Link(handle);

  pluginDefMap_[fileName] = PPluginDef( new PluginDef(plugin) );
}





} } } //namespace avs::plugin::manager
