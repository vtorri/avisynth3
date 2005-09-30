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


//avisynth include
#include "parser.h"
#include "vmstate.h"
#include "grammar/script.h"
#include "../linker/core/plugin.h"
#include "../filters/source/messageclip.h"

//boost include
#include <boost/variant/get.hpp>

//stl include
#include <sstream>



namespace avs { namespace parser {



StatementCode Parser::operator()(std::string const& src)
{

  //using namespace lazy;
  using namespace phoenix;
  using namespace avs::parser::grammar;

  CodeCouple code;
  LocalContext localCtxt;
  GlobalContext globalCtxt;

  globalCtxt.get<1>().AddPlugin(linker::core::Plugin::Get());

  Script script;
  //Statement statement;
  //Expression expression;

  parse(src.c_str(), 
      script( CodeCouple(), boost::ref(localCtxt), boost::ref(globalCtxt) )
      //statement( CodeCouple(), boost::ref(localCtxt), boost::ref(globalCtxt), 'c' )
      //expression( value::Expression(), boost::ref(localCtxt), boost::ref(globalCtxt) )
      [            
        var(code) += arg1
      ]
      , spirit::blank_p);

  return code;
}


PClip Parser::operator ()(std::string const& src, PEnvironment const& env)
{
  
  StatementCode code = operator()(src);

  VMState state(env);  
  //state.push(1);

  code(state); 

  //if top stack value is a clip, we return it
  if ( state.size() != 0 && boost::get<PClip>(&state.top()) != NULL )
    return boost::get<PClip>(state.top());

  //return boost::get<PClip>(state.top());
  std::stringstream stream;

  stream << "parsed:";

  for(int i = 0; i < state.size(); ++i)
    stream << ' '  << state[i];  

  return filters::MessageClip::Create(stream.str(), env );
}


} } //namespace avs::parser
