// Avisynth v3.0 alpha.  Copyright 2003-2006 David Pierre - Ben Rudiak-Gould et al.
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
#include "grammar.h"
#include "operators.h"
#include "../functor/var.h"
#include "../functor/pusher.h"
#include "../functor/literal.h"
#include "../functor/assigner.h"

//spirit includes
//#include <boost/spirit/phoenix/statements.hpp>      //for operator,
//#include <boost/spirit/utility/escape_char.hpp>   //for c_escape_ch_p, the day it starts converting



namespace avs { namespace parser { namespace lowlvl {


template <>
Grammar::definition<Scanner>::definition(Grammar const & self)
{

  using namespace phoenix;
  using namespace functor;

  top
      =   '!'
      >>  lit
      |   "op"
      >>  Operators::instance
          [
            var(self.packer) += arg1
          ]
      |   stack
      ;

  lit
      =   spirit::strict_real_p
          [
            var(self.packer) += construct_<pusher<literal<double> > >(arg1)
          ]
      |   spirit::int_p
          [
            var(self.packer) += construct_<pusher<literal<int> > >(arg1)
          ]
      |   stringLiteral
          [
            var(self.packer) += construct_<pusher<literal<std::string> > >(arg1)
          ]
      |   spirit::str_p("true")
          [
            var(self.packer) += construct_<pusher<literal<bool> > >(val(true))
          ]
      |   spirit::str_p("false")
          [
            var(self.packer) += construct_<pusher<literal<bool> > >(val(false))
          ]
      ;


  stringLiteral
      =   spirit::lexeme_d
          [
              '"'
          >> *(   spirit::str_p("\\n")
                  [
                    stringLiteral.value += val('\n')
                  ]
              |   ( spirit::anychar_p - '"' )
                  [
                    stringLiteral.value += *arg1
                  ]
              )
          >>  '"'
          ]
      ;

  stack
      =   '['
      >>  spirit::int_p
          [
            stack.value = arg1
          ]
      >>  ']'
      >>  (   spirit::ch_p('=')
              [
                var(self.packer) += construct_<assigner<LocalVar> >(stack.value)
              ]
          |   spirit::eps_p
              [
                var(self.packer) += construct_<pusher<LocalVar> >(stack.value)
              ]
          )
      ;

}


} } } //namespace avs::parser::lowlvl
