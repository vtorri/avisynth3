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
#include "statement.h"
#include "../lazy/get.h"
#include "../lazy/ref.h"
#include "../lazy/tuple.h"
#include "../action/check.h"
#include "../action/action.h"
#include "../functor/if.h"
#include "../functor/while.h"
#include "../functor/literal.h"

//spirit includes
#include <boost/spirit/core.hpp>
#include <boost/spirit/phoenix/statements.hpp>      //for operator,


namespace avs { namespace parser { namespace grammar {



template <>
Statement::definition<Scanner>::definition(Statement const & self)
{

  using namespace lazy;
  using namespace functor;
  using namespace phoenix;

  
  top
      =   statement( CodeCouple(), self.localCtxt )
          [ 
            self.value = arg1 
          ]
      ;

  statement
      =   (   expression( value::Expression(), statement.localCtxt, self.globalCtxt )
              [
                bind(&action::Action::ExprStatement)(statement.value, statement.localCtxt, arg1)                        
              ]
          |   createVar( ref(first(statement.localCtxt)) )  //pass local VarTable
          |   ifStatement
          // |   whileStatement
          |   returnStatement
          )
      >>  (   spirit::eol_p           //statement are normally ended by a newline
          |   spirit::eps_p( '}' )    //but an end of block would do too
          )
      ;

  createVar
      =   (   spirit::str_p("global")
              [
                createVar.table = ref(first(self.globalCtxt))    //set global VarTable as the table to use
              ]
          |   !   spirit::str_p("local")     //optional local keyword
          )
      >>  name
          [
            createVar.name = construct_<std::string>(arg1, arg2)
          ]
      >>  '='
      >>  expression( value::Expression(), statement.localCtxt, self.globalCtxt )
          [
            bind(&action::Action::CreateVarStatement)
                (statement.value, statement.localCtxt, arg1, createVar.table, createVar.name)
          ]
      ;

  ifStatement
      =   spirit::str_p("if")
      >>  conditionBlock
          [
            statement.value += arg1
          ]
      >>  subContextBlock( CodeCouple(), get(statement.localCtxt) )
          [
            ifStatement.value = arg1
          ]          
      >>  (   *   spirit::eol_p
          >>  spirit::str_p("else")
          >>  subContextBlock( CodeCouple(), get(statement.localCtxt) )
              [
                statement.value -= construct_<functor::IfThenElse>(ifStatement.value, arg1)
              ]
          |   spirit::eps_p
              [
                statement.value -= construct_<functor::IfThen>(ifStatement.value)
              ]
          )
      ;

  whileStatement
      =   spirit::str_p("while")
      >>  conditionBlock
          [
            whileStatement.value = arg1
          ]
      >>  subContextBlock( CodeCouple(), get(statement.localCtxt) )
          [
            statement.value -= construct_<functor::WhileDo>(whileStatement.value, arg1)
          ]
      ;

  conditionBlock
      =   '('
      >>  expression( value::Expression(), statement.localCtxt, self.globalCtxt )
          [
            bind(&action::Check::TypeIsExpected)(second(arg1), val('b')),
            conditionBlock.value += first(arg1),
            --second(statement.localCtxt)         //reports that the bool value is consumed
          ]
      >>  ')'
      ;

  subContextBlock
      =   spirit::eps_p
          [    //get rid of last inherited from upper context
            third(subContextBlock.localCtxt) = val( boost::detail::none_t() )
          ]
      >>  *   spirit::eol_p   //skip newlines
      >>  (   '{'     
          >> *(   statement( CodeCouple(), ref(subContextBlock.localCtxt) )   //a block of statement delimited by { }
                  [
                    subContextBlock.value += arg1
                  ]
              |   spirit::eol_p
              )
          >>  '}'
          |   statement( CodeCouple(), ref(subContextBlock.localCtxt) )       //or a single one
              [
                subContextBlock.value += arg1
              ]
          )
      ;

  returnStatement
      =   spirit::str_p("return")
      >>  (   spirit::str_p("self")
              [
                bind(&action::Check::TRecurseAllowed)(! self.termRecInfo)
              ]
          >>  argList( 0 )
              [
                second(get(self.termRecInfo)) = val(true),                //report that terminal recursivity is used
                statement.value -= construct_<literal<OpType> >(RECURSE)
              ]
          |   expression( value::Expression(), statement.localCtxt, self.globalCtxt )
              [
                bind(&action::Check::ReturnTypeIsExpected)(second(arg1), self.returnTypeExpected),
                statement.value += first(arg1),
                statement.value -= construct_<literal<OpType> >(RETURN)
              ]
          )
      ;

  argList
      =   '('
      >> !(   expression( value::Expression(), statement.localCtxt, self.globalCtxt )
              [
                //check the type is the one expected by prototype of outer function
                bind(&action::Check::TypeIsExpected)(second(arg1), first(get(self.termRecInfo))[argList.value]),
                statement.value += first(arg1),
                ++argList.value
              ]
          %   ','
      )
      >>  ')'
      ;

}



} } } //namespace avs::parser::grammar
