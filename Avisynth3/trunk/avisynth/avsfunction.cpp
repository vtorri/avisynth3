// Avisynth v3.0 alpha.  Copyright 2002 Ben Rudiak-Gould et al.
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


#include "avsfunction.h"
#include "plugin.h"



PPlugin PluginFunction::GetMotherPlugin() const { mother.AddRef(); return &mother; }

CoreFunction::CoreFunction(const string& name) : AVSFunction(name) { CorePlugin::core.RegisterFunction(*this); }



AVSValue LinkedFunction::Call(const ArgVector& args) throw(std::invalid_argument)
{
  const DescriptionPrototype& prototype = GetPrototype();

  if ( args.size() > prototype.size() )
    throw std::invalid_argument("too much arguments");

  ArgVector::const_iterator a_it = args.begin();
  DescriptionPrototype::const_iterator p_it = prototype.begin();
  while ( a_it != args.end() )   //while there are args
  {
    if ( a_it++->type() != p_it++->Type() )           //if type mismatch
      throw std::invalid_argument("type mismatch");   //exception
  }      
  
  if ( p_it != prototype.end() && ! p_it->IsOptional() )
    throw std::invalid_argument("not enough arguments");

  ArgVector filledArgs = args;
  while( p_it != prototype.end() )
  {
    filledArgs.push_back( p_it->GetDefault() );    //complete with default values
  }

  return operator()(filledArgs, GetEnvironment());

}





AVSValue ReorderingCallBackFunction::operator()(const ArgVector& args, PEnvironment env) const
{
  const DescriptionPrototype& callBackPrototype = CallBackFunction::GetPrototype();
 
  ArgVector reorderedArgs(callBackPrototype.begin(), callBackPrototype.end());

  //we are protected by Call and the factory method that pattern and args
  //are compatible
  for(int i = args.size(); i-- > 0; )
  {
    reorderedArgs[pattern[i].GetPosition()] = pattern[i](args[i]);  //put arg at its place and type cast    
  }
  //directly call the operator () from the parent (avoid a redundant Call)
  return CallBackFunction::operator()(reorderedArgs, env);
}
