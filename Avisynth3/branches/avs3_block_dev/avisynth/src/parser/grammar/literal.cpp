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
#include "literal.h"
#include "../lazy/tuple.h"
#include "../functor/pusher.h"
#include "../functor/literal.h"


//spirit includes
#include <boost/spirit/core.hpp>
#include <boost/spirit/phoenix/statements.hpp>      //for operator,
//#include <boost/spirit/utility/escape_char.hpp>   //for c_escape_ch_p, the day it starts converting



namespace avs { namespace parser { namespace grammar {



template <>
Literal::definition<Scanner>::definition(Literal const & self)
{

  using namespace lazy;
  using namespace phoenix;
  using namespace functor;


  top
      =   spirit::strict_real_p
          [
            first(self.value) = construct_<pusher<literal<double> > >(arg1),
            second(self.value) = val('d')
          ]
      |   spirit::int_p
          [
            first(self.value) = construct_<pusher<literal<int> > >(arg1),
            second(self.value) = val('i')
          ]
      |   stringLiteral
          [
            first(self.value) = construct_<pusher<literal<std::string> > >(arg1),
            second(self.value) = val('s')
          ]
      |   (   spirit::str_p("true")
              [
                first(self.value) = construct_<pusher<literal<bool> > >(val(true))
              ]
          |   spirit::str_p("false")
              [
                first(self.value) = construct_<pusher<literal<bool> > >(val(false))
              ]
          )
          [
            second(self.value) = val('b')
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

}



} } } //namespace avs::parser::grammar
