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


#include "avsfunction.h"
#include "prototype.h"




AVSValue AVSFunction::Call(const ArgVector& args, VarTable& table, ScriptEnvironment& env)
{
  const DescriptionPrototype& prototype = GetPrototype();

  //if more args than in proto
  if ( args.size() > prototype.size() )
    throw std::invalid_argument(GetName() + ": too much arguments");  //exception
  
  DescriptionPrototype::const_iterator p_it = prototype.begin();
  for(ArgVector::const_iterator a_it = args.begin(); a_it != args.end(); )   //loop over args  
    if ( a_it++->type() != p_it++->Type() )                                  //if type mismatch
      throw std::invalid_argument(GetName() + ": type mismatch");            //exception        

  //if more in proto and not optional  
  if ( p_it != prototype.end() && ! p_it->IsOptional() )
    throw std::invalid_argument(GetName() + ": not enough arguments");  //exception

  ArgVector filledArgs = args;             //vector to fill with defaults
  while( p_it != prototype.end() )         //while there are some
    filledArgs.push_back( p_it->GetDefault() );    //complete with default values  

  return operator()(filledArgs, table, env);

}



/*

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
*/