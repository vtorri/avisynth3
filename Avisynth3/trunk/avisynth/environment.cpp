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


#include "environment.h"
#include "avsfunction.h"
#include "plugin.h"
#include <algorithm>



void * ScriptEnvironment::malloc(size_type size)
{
  APMemoryPiece apmp = new MemoryPiece(size);   //create memory block
  void * result = apmp->get();         
  allocMap[result] = apmp;            //store it in alloc map (AP ownership transferred into map)
  MemoryUsedIncreased(apmp->size());  //increase memory usage (env may react)
  return result;                      //return result
}

void ScriptEnvironment::free(void * ptr)
{
  AllocationMap::iterator it = allocMap.find(ptr);  //search for ptr
  if ( it != allocMap.end() )                       //if found
  {
    MemoryUsedReleased( it->second()->size() );     //decrease memory usage
    allocMap.erase(it);                             //release block
  }
}



ScriptEnvironment::ScriptEnvironment()
{
  pluginVector.push_back(&CorePlugin::core);  //register the core plugin as first env plugin
}

ScriptEnvironment::~ScriptEnvironment()
{
  for(AllocationMap::iterator it = allocMap.begin(); it != allocMap.end(); ++it)
    delete it->second;
}


void ScriptEnvironment::LoadPlugin(const string& pathName)
{
  PPlugin plugin = NativePlugin::GetPlugin(pathName);  //fetch (create) the plugin
  //if not already included
  if ( find(pluginVector.begin(), pluginVector.end(), plugin) == pluginVector.end() )
    pluginVector.push_back(plugin);  //we add it
}



LinkResult ScriptEnvironment::Link(const string& name, const LinkagePrototype& link, bool ImplicitLastAllowed)
{
  SearchResult result;
  //in each plugin
  for (PluginVector::const_iterator p_it = pluginVector.begin(); p_it != pluginVector.end(); ++p_it )
  {
    (*p_it)->FunctionSearch(result, name, link); //we search the wanted function
  }

  if ( result.empty() )                 //if nothing found
    return make_pair(PAVSFunction(), false);  //we return a 'NULL' result  
  
  //find the MatchResult with the highest priority
  SearchResult::iterator it = max_element(result.begin(), result.end());

  bool UseImplicitLast = it->UseImplicitLast();  //fetch if implicit last is needed

  if ( UseImplicitLast && ! ImplicitLastAllowed) //implicit last needed but illegal
    return make_pair(PAVSFunction(), false);     //failure result

  PEnvironment env = this;
  env->AddRef();   
  return make_pair(it->Wrap(env), UseImplicitLast);
}