// Avisynth v3.0 alpha.  Copyright 2004 Ben Rudiak-Gould et al.
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
#include "functiontable.h"
#include "scriptfunction.h"
#include "../linker/plugin.h"
#include "../linker/function.h"
#include "functor/functioncall.h"


namespace avs { namespace parser {



void FunctionTable::AddPlugin(linker::PPlugin const& plugin)
{
  linker::FunctionList list = plugin->GetFunctionList();

  for( linker::FunctionList::iterator it = list.begin(); it != list.end(); ++it )
    AddFunction( *it );
};


void FunctionTable::AddFunction(linker::PFunction const& function)
{
  FunctionPool * pool = spirit::find(*this, function->GetName());
  if ( pool == NULL )
    pool = spirit::add(*this, function->GetName());

  pool->Add(function);
}


void FunctionTable::AddScriptFunction(char type, std::string const& name, std::string const& prototype, StatementCode const& code)
{
  ElementalOperation op;

  if ( type == 'v' )
    op = functor::VoidFunctionCall(prototype.size(), code);
  else op = functor::FunctionCall(prototype.size(), code);

  linker::PFunction funct( new ScriptFunction(type, name, prototype, op ) );

  AddFunction( funct );
}

} } //namespace avs::parser
