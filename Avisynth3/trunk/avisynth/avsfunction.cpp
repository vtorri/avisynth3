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

AVSValue AVSFunction::operator() (const NamedArgVector& args) const
{
  //the error we throw when args are not what they should
  static InternalError PARSER_ERR = "Parser Error: passed arguments does not match function prototype";

  const Prototype& prototype = GetPrototype();

  int argCount = prototype.size();
  //build a vector of the goood size to order values into
  ArgVector sortedArgs(argCount);  
  //fill it with default values
  for(int i = 0; i < argCount; ++i)
    sortedArgs.push_back(prototype[i]);

  //now the ugly part: check that args fits the job :/
  i = 0;  //stupid VC6 keeps i defined from the for :'(
  //while it matches we copy in
  while( i < args.size() && prototype[i].Match(args[i]) )
  { 
    sortedArgs[i] = args[i];
    ++i;
  }
  //now we test the stop condition of the while
  if (i != args.size()) //if == it's finished and totally matched 
  {
    if (! prototype[i].IsOptional() ) //if not stopped on optional: no match
      throw PARSER_ERR;
    //now we have to reorder named optional args
    while (i < args.size())
    {
      int pos = prototype.IndexOf(args[i]);
      if (pos == -1 || ! prototype[pos].Match(args[i]))  //ie not found or not matching
        throw PARSER_ERR;
      sortedArgs[pos] = args[i];  //we place it at the good place
    }
  }
  return Process(sortedArgs);  //and finally we pass the job to Process
}




CorePlugin * CorePlugin::GetCore()
{
  static CorePlugin * core = new CorePlugin();
  //we don't have to worry about deleting it here, the env will do it.
  return core;
}


const string& CorePlugin::GetName() const
{
  static const string name = "Avisynth Core";
  return name;
}

const string& CorePlugin::GetAuthor() const
{
  static const string author = "Ben Rubiak Gould et al.";
  return author;
}

const string& CorePlugin::GetDescription() const
{
  static const string description = "Set of all avisynth internals";
  return description;
}
