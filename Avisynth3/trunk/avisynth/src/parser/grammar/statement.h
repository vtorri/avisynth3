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


#ifndef __AVS_PARSER_GRAMMAR_STATEMENT_H__
#define __AVS_PARSER_GRAMMAR_STATEMENT_H__

//avisynth includes
#include "name.h"
#include "action.h"
#include "expression.h"
#include "../lazy/get.h"
#include "../codecouple.h"
#include "../functor/if.h"


namespace avs { namespace parser { namespace grammar {



namespace closure {


/////////////////////////////////////////////////////////////////////////////////
//  closure::Statement
//
//  closure for the Statement grammar
//
struct Statement : spirit::closure
    < Statement
    , CodeCouple
    , boost::reference_wrapper<LocalContext>
    , boost::reference_wrapper<GlobalContext>
    , char
    , boost::optional<value::TRecurseInfo>
    >
{
  member1 value;
  member2 localCtxt;
  member3 globalCtxt;
  member4 returnTypeExpected;
  member5 termRecInfo;
};


/////////////////////////////////////////////////////////////////////////////////
//  closure::InnerStatement
//
//  closure for the statement rule within the Statement grammar
//
struct InnerStatement : spirit::closure
    < InnerStatement
    , CodeCouple
    , boost::reference_wrapper<LocalContext>
    >
{
  member1 value;
  member2 localCtxt;
};


struct CreateVar : spirit::closure
    < CreateVar
    , boost::reference_wrapper<VarTable>
    , std::string
    >
{
  member1 table;
  member2 name;
};


struct StatementBlock : spirit::closure
    < StatementBlock
    , CodeCouple
    , LocalContext
    >
{
  member1 value;
  member2 localCtxt;
};


} //namespace closure




struct Statement : public spirit::grammar<Statement, closure::Statement::context_t>
{

  template <typename ScannerT>
  struct definition
  {

    definition(Statement const& self)
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
                    bind(&Action::ExprStatement)(statement.value, statement.localCtxt, arg1)                        
                  ]
              |   createVar( ref(first(statement.localCtxt)) )  //pass local VarTable
              |   ifStatement
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
                bind(&Action::CreateVarStatement)
                    (statement.value, statement.localCtxt, arg1, createVar.table, createVar.name)
              ]
          ;

      ifStatement
          =   spirit::str_p("if")
          >>  '('
          >>  expression( value::Expression(), statement.localCtxt, self.globalCtxt )
              [
                bind(&Check::TypeIsExpected)(second(arg1), val('b')),
                statement.value += first(arg1),
                --second(statement.localCtxt)                          //report that the if consumes the bool value
              ]
          >>  ')'
          >>  block( CodeCouple(), get(statement.localCtxt) )
              [
                ifStatement.value = arg1
              ]          
          >>  (   *   spirit::eol_p
              >>  spirit::str_p("else")
              >>  block( CodeCouple(), get(statement.localCtxt) )
                  [
                    statement.value -= construct_<functor::IfThenElse>(ifStatement.value, arg1)
                  ]
              |   spirit::eps_p
                  [
                    statement.value -= construct_<functor::IfThen>(ifStatement.value)
                  ]
              )
          ;

      block
          =   *   spirit::eol_p
          >>  '{' //NB: for now last is inherited from upper context.... FIX ME LATER
          >> *(   statement( CodeCouple(), ref(block.localCtxt) )
                  [
                    block.value += arg1
                  ]
              |   spirit::eol_p
              )
          >>  '}'
          ;

      returnStatement
          =   spirit::str_p("return")
          >>  (   spirit::str_p("self")
                  [
                    bind(&Check::TRecurseAllowed)(! self.termRecInfo)
                  ]
              >>  argList( 0 )
                  [
                    second(get(self.termRecInfo)) = val(true),                //report that terminal recursivity is used
                    statement.value -= construct_<literal<OpType> >(RECURSE)
                  ]
              |   expression( value::Expression(), statement.localCtxt, self.globalCtxt )
                  [
                    bind(&Check::ReturnTypeIsExpected)(second(arg1), self.returnTypeExpected),
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
                    bind(&Check::TypeIsExpected)(second(arg1), first(get(self.termRecInfo))[argList.value]),
                    statement.value += first(arg1),
                    ++argList.value
                  ]
              %   ','
              )
          >>  ')'
          ;
    }

    spirit::rule<ScannerT> const & start() const { return top; }


  private:

    spirit::rule<ScannerT> top;
    spirit::rule<ScannerT, closure::InnerStatement::context_t> statement;
    spirit::rule<ScannerT, closure::CreateVar::context_t> createVar;
    spirit::rule<ScannerT, closure::Value<StatementCode>::context_t> ifStatement;
    spirit::rule<ScannerT, closure::StatementBlock::context_t> block;
    spirit::rule<ScannerT> returnStatement;
    spirit::rule<ScannerT, closure::Value<int>::context_t> argList;

    Name name;
    Expression expression;

  };

};



} } } //namespace avs::parser::grammar

#endif //__AVS_PARSER_GRAMMAR_STATEMENT_H__
