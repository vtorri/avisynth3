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



#include "linker.h"


const ArgumentMatcher::CastMap ArgumentMatcher::castMap = CreateCastMap();

ArgumentMatcher::CastMap ArgumentMatcher::CreateCastMap()
{
  CastMap result;
  result[CastPrototype(typeid(int), typeid(float))] = &itf;
  return result;
};


ArgumentMatcher::ArgumentMatcher(const DescriptionArgument& descript, const LinkageArgument& link)  
 : success(false)
{
  if ( link.GetName().empty()     //either link has empty name  or descript optional and names match
       || ( descript.IsOptional() && descript.GetName() == link.GetName() )
     )
  {
    success = true;
    if ( link.Type() != descript.Type() ) //if equals, success and everything is ok     
    {
      //we search for a valid cast
      CastPrototype cp(link.Type(), descript.Type());

      CastMap::const_iterator it = castMap.find(cp);
      if ( it == castMap.end() )
        success = false;           //no such cast : failed
      else         
        caster = it->second;    //else we use the specified  caster      
    }
  }
}




void PrototypeMatcher::TryToMatch(const DescriptionPrototype& descript, const LinkagePrototype& link)
{
  int size_d = descript.size();
  int size_l = link.size();
  
  if ( size_d < size_l)  //not enough arguments in the function : cannot match
    return;              //we abort

  //first step, we try to match args of same position
  int i = 0;
  while( i < size_l && AddToPattern(ArgumentMatcher(descript[i], link[i]), i) ) { ++i; }

  //second step : take care of the rest
  if ( i == size_l ) //is there a rest ?
  {                  //everything from link used
    //we must check that we have reached the optional part of desript
    if ( i == size_d || descript[i].IsOptional() )   //at end or optional
      match = true;         //then matched    
  } else {           //link is not consumed
    //position where we start searching for args
    //those before were already consumed by the first step
    DescriptionPrototype::const_iterator search_start = descript.begin() + i;
    DescriptionPrototype::const_iterator it;
    while ( i < size_l &&   //while link not consumed...
            //...and we can find an arg of the same name in descript...
            ( it = find(search_start, descript.end(), link[i].GetName()), it != descript.end() )
              //...which we manage to match...
              && AddToPattern( ArgumentMatcher(*it, link[i]), it - descript.begin() )
              ) { ++i; }  //...we continue
    //test the stop condition of the above
    if ( i == size_l )    //did we consume link
      match = true;       //then matched    
  }
}

PrototypeMatcher::PrototypeMatcher(const DescriptionPrototype& descript, const LinkagePrototype& link)
 : match(false), useImplicitLast(false)
{
  TryToMatch(descript, link);             //1st matching pass

  if ( ! Success() )                      //if failed
  {
    //arg to add in front of the passed LinkagePrototype to search for implicit last
    static const LinkageArgument clipArg(typeid(PClip));

    useImplicitLast = true;               //we use implicit last
    LinkagePrototype copy = link;         //copy link
    copy.insert(copy.begin(), clipArg);   //insert a clip arg in first position of the copy
    
    clear();                              //reset pattern for 2nd pass
    TryToMatch(descript, copy);           //retry matching with the modified prototype
  }
}



bool ReorderPattern::IsTrivial() const
{  
  for( int i = size(); i--> 0; )
    if ( operator[](i).GetPosition() != i )
      return false;
  return true;
}

DescriptionPrototype Adapt(const DescriptionPrototype& old) const
{
  DescriptionPrototype result;
  for(const_iterator it = begin(); it != end(); ++it)
  {
    DescriptionArgument& oldArg = old[it->GetPosition()];
    //result.push_back();  //take the pointed Descript arg to make the new proto
  }
  return result;
}






