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


#ifndef __AVS_PARSER_FUNCTION_TABLE_H__
#define __AVS_PARSER_FUNCTION_TABLE_H__

//avisynth includes
#include "pool.h"
#include "forward.h"
#include "../forward.h"           //for StatementCode declaration

//spirit include
#include <boost/spirit/symbols.hpp>

//stl include
#include <map>

namespace spirit = boost::spirit;



namespace avs { namespace parser { namespace function {



////////////////////////////////////////////////////////////////////////////////////////
//  function::Table
//
//  symbols table mapping function names to the pool of functions of that name
//
class Table : public spirit::symbols<Pool>
{

  typedef std::map<FunctionId, boost::shared_ptr<ScriptFunction> > UndefScriptFunctionMap;

  UndefScriptFunctionMap undefsMap_;


public:

  //add all functions from given plugin
  void AddPlugin(linker::PPlugin const& plugin);

  //add given function
  void AddFunction(linker::PFunction const& function);

  //declaration and definition of script functions
  void DeclareScriptFunction(FunctionId const& id);
  void DefineScriptFunction(FunctionId const& id, StatementCode const& code, bool termRecursive);

};



} } } //namespace avs::parser::function

#endif //__AVS_PARSER_FUNCTION_TABLE_H__
