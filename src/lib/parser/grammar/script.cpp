// Avisynth v3.0 alpha.  Copyright 2005 David Pierre - Ben Rudiak-Gould et al.
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
#include "types.h"
#include "script.h"
#include "../lazy/ref.h"
#include "../lazy/tuple.h"

//spirit includes
#include <boost/spirit/core.hpp>
#include <boost/spirit/dynamic/lazy.hpp>            //for lazy_p
#include <boost/spirit/phoenix/statements.hpp>      //for operator,



namespace avs { namespace parser { namespace grammar {


template <>
Script::definition<Scanner>::definition(Script const & self)
{

  using namespace lazy;
  using namespace phoenix;


  top
      =  *(   statement( CodeCouple(), self.localCtxt, self.globalCtxt, 'c' )
              [
                self.value += arg1       //accumulate code
              ]
          |   spirit::str_p("function")
          >>  function
          |   spirit::eol_p
          )
      ;

  function
      =   Types::instance
          [
            first(function.ident) = arg1,
            second(function.localCtxt) = val(0)
          ]
      >>  name
          [
            second(function.ident) = construct_<std::string>(arg1, arg2)
          ]
      >>  '('
      >>  !(  arg   %   ','   )
      >>  spirit::ch_p(')')
          [
            bind(&function::Table::DeclareScriptFunction)( second(self.globalCtxt), function.ident ),
            function.termRecursive = val(false)       //init recursive flag
          ]
      >>  functionBody
      >>  spirit::eol_p
          [
            bind(&function::Table::DefineScriptFunction)
                ( second(self.globalCtxt), function.ident, function.code, function.termRecursive )
          ]
      ;

  arg
      =   Types::instance
          [
            arg.value = arg1
          ]
      >>  (   name - spirit::lazy_p( first(function.localCtxt) )  )
          [
            bind(&VarTable::DefineVar)(first(function.localCtxt), construct_<std::string>(arg1, arg2), arg.value),
            third(function.ident) += arg.value,
            ++second(function.localCtxt)
          ]
      ;

  functionBody
      =   *   spirit::eol_p
      >>  '{'
      >> *(   statement
                  ( CodeCouple()
                  , ref_(function.localCtxt)
                  , self.globalCtxt
                  , first(function.ident)
                  , construct_<value::TRecurseInfo>( ref_(third(function.ident)), ref_(function.termRecursive) )
                  )
              [
                function.code += arg1
              ]
          |   spirit::eol_p
          )
      >>  '}'
      ;

}


} } } //namespace avs::parser::grammar
