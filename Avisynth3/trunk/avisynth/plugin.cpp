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


#include "plugin.h"




bool MatchResult::operator <(const MatchResult& other) const
{
  //if not same comportment considering implicit last
  if ( UseImplicitLast() != other.UseImplicitLast() )
    return UseImplicitLast();   //self is < if it use it


}


void Plugin::AddFunction(AVSFunction& funct) throw(std::invalid_argument)
{
  const type_info * type = ReturnTypeOf(funct.GetName());
  //if not NULL (aka is an overload) and different.... error
  if ( type != NULL && *type != funct.GetReturnType() )
    throw std::invalid_argument(GetName() + ": illegal overload of function " + funct.GetName());

  list.push_back(&funct);
}

const type_info * Plugin::ReturnTypeOf(const string& name) const
{
  for(FunctionList::const_iterator it = list.begin(); it != list.end(); ++it)
  {
    if ( (*it)->GetName() == name )
      return &(*it)->GetReturnType();
  }
  //not found if we are here
  return NULL;
}

void Plugin::FunctionSearch(SearchResult& result, const string& name, const LinkagePrototype& link)
{
  for(FunctionList::iterator it = list.begin(); it != list.end(); ++it)
  {    
    if ( (*it)->GetName() == name)
    {
      MatchResult matcher(**it, link);
      if ( matcher.Success() )    
        result.push_back(matcher);
    }    
  }
}

