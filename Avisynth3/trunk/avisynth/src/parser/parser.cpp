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


//avisynth include
#include "parser.h"
#include "vmcode.h"
#include "grammar/statement.h"
#include "../core/runtime_environment.h"
#include "../filters/source/staticimage.h"
#include "../linker/core/plugin.h"

//stl include
#include <sstream>



namespace avs { namespace parser {



PClip Parser::operator ()(std::string const& src)
{
  
  using namespace phoenix;
  using namespace avs::parser::grammar;

  //Expression expression;
  PEnvironment env = RuntimeEnvironment::Create(10000000);

  int stackSize = 0;
  VMState state(env);  
  VarTable varTable;
  FunctionTable functionTable;

  functionTable.AddPlugin(linker::core::Plugin::Get());

  Statement statement(stackSize, varTable, functionTable);
  //Variable variable(varTable);

  VMCode code;
  //std::string types;

  parse(src.c_str(), 
   *( statement
      [
        var(code) += arg1//,
    //    var(types) += bind(&TypedCode::type)(arg1)
      ]
      >> spirit::eol_p
    ), spirit::blank_p);


  code(state);



  std::stringstream stream;

  stream << "parsed:";

  for(int i = 0; i < state.size(); ++i)
    stream << ' ' /*<< types[i] << ":"*/ << state[i];

  return filters::StaticImage::CreateMessageClip(stream.str(), env );
}


} } //namespace avs::parser