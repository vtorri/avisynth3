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
#include "grammar/literal.h"
#include "../core/runtime_environment.h"
#include "../filters/source/staticimage.h"

//stl include
#include <sstream>

using namespace phoenix;
using namespace avs::parser::grammar;


namespace avs { namespace parser {



PClip Parser::operator ()(std::string const& src)
{
  
  Literal literal;

  VMCode code;
  std::string types;

  parse(src.c_str(), 
   *( literal
      [
        var(code) += bind(&TypedOp::op)(arg1),
        var(types) += bind(&TypedOp::type)(arg1)
      ]
    ), spirit::space_p);

  Stack stack;
  code(stack);

  std::stringstream stream;

  stream << "parsed:";

  for(int i = 0; i < stack.size(); ++i)
    stream << ' ' << types[i] << ":" << stack.stack_[i];

  return filters::StaticImage::CreateMessageClip(stream.str(), RuntimeEnvironment::Create(10000000) );
}


} } //namespace avs::parser
