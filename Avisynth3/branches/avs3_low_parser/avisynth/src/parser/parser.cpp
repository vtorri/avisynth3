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
#include "parser.h"
#include "lowlvl/grammar.h"
#include "../filters/source/messageclip.h"

//boost include
#include <boost/variant/get.hpp>

//stl include
#include <sstream>



namespace avs { namespace parser {



CodePacker Parser::operator()(std::string const& src)
{
  using namespace phoenix;

  CodePacker packer;

  lowlvl::Grammar grammar(packer);

  parse( src.c_str()
       , grammar
       , spirit::blank_p
       );

  return packer;

}


PClip Parser::operator ()(std::string const& src, PEnvironment const& env)
{
  CodePacker code = operator()(src);

  VirtualMachine vm(env);

  vm.Run(code.begin());

  //if top stack value is a clip, we return it
  if ( vm.stack.size() != 0 && boost::get<PClip>(&vm.stack.front()) != NULL )
    return boost::get<PClip>(vm.stack.front());

  //return boost::get<PClip>(state.top());
  std::stringstream stream;

  stream << "parsed:";

  for( VirtualMachine::Stack::iterator it = vm.stack.begin(), end = vm.stack.end(); it != end; ++it )
    stream << ' '  << *it;  

  return filters::MessageClip::Create(stream.str(), env );
}



} } //namespace avs::parser
