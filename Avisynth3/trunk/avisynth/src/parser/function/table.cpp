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
#include "table.h"
#include "../vmcode.h"
#include "scriptfunction.h"
#include "../../linker/plugin.h"
#include "../../linker/function.h"

//boost include
#include <boost/tuple/tuple_comparison.hpp>


namespace avs { namespace parser { namespace function {




void Table::AddPlugin(linker::PPlugin const& plugin)
{
  linker::FunctionList list = plugin->GetFunctionList();

  for( linker::FunctionList::iterator it = list.begin(); it != list.end(); ++it )
    AddFunction( *it );
};


void Table::AddFunction(linker::PFunction const& function)
{
  Pool * pool = spirit::find(*this, function->GetName());
  if ( pool == NULL )
    pool = spirit::add(*this, function->GetName());

  pool->Add(function);
}


void Table::DeclareScriptFunction(FunctionId const& id)
{
  //if not already declared
  if ( undefsMap_.find(id) == undefsMap_.end() )
  {
    //create the script function
    boost::shared_ptr<ScriptFunction> funct( new ScriptFunction(id) );

    undefsMap_[id] = funct;   //add it to undef map

    AddFunction( funct );     //and add to the symbols table
  }
}
  

void Table::DefineScriptFunction(FunctionId const& id, StatementCode const& code, bool termRecursive)
{
  //fetch script function from undefs map
  UndefScriptFunctionMap::iterator it = undefsMap_.find(id);

  it->second->Define(code, termRecursive);   //define it

  undefsMap_.erase(it);  //and remove it from undefs map
}


} } } //namespace avs::parser::function
