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


#ifndef __AVS_PARSER_GRAMMAR_STATEMENT_H__
#define __AVS_PARSER_GRAMMAR_STATEMENT_H__

//avisynth includes
#include "name.h"
#include "expression.h"
#include "../optype.h"
#include "../codecouple.h"
#include "../functor/if.h"
#include "../functor/popper.h"
#include "../functor/swapper.h"


namespace avs { namespace parser { namespace grammar {



namespace value { 
  
typedef boost::tuples::tuple<bool, boost::optional<int> > StackingStatement;

}

namespace closure {


/////////////////////////////////////////////////////////////////////////////////
//  closure::Statement
//
//  closure for the Statement grammar
//
struct Statement : spirit::closure
    < Statement
    , CodeCouple
    , boost::reference_wrapper<VarTable>
    , boost::reference_wrapper<boost::optional<int> >
    , boost::reference_wrapper<VarTable>
    , boost::reference_wrapper<FunctionTable>
    , char
    >
{
  member1 value;
  member2 localTable;
  member3 last;
  member4 globalTable;
  member5 functionTable;
  member6 returnTypeExpected;
};


/////////////////////////////////////////////////////////////////////////////////
//  closure::InnerStatement
//
//  closure for the statement rule within the Statement grammar
//
struct InnerStatement : spirit::closure
    < InnerStatement
    , CodeCouple
    , boost::reference_wrapper<VarTable>
    , boost::reference_wrapper<boost::optional<int> >
    >
{
  member1 value;
  member2 localTable;
  member3 last;
};


struct CreateVar : spirit::closure
    < CreateVar
    , bool
    , boost::reference_wrapper<VarTable>
    , std::string
    , int
    >
{
  member1 globalVar;
  member2 varTable;
  member3 name;
  member4 index;
};


struct StatementBlock : spirit::closure
    < StatementBlock
    , CodeCouple
    , VarTable
    , boost::optional<int>
    >
{
  member1 value;
  member2 varTable;
  member3 last;
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
          =   statement( CodeCouple(), self.localTable, self.last )
              [ 
                self.value = arg1 
              ]
          ;

      statement
          =   (   stackingStatement( value::StackingStatement(true) )
                  [
                    if_( unwrap(statement.last) )   //if last was defined
                    [
                      if_( first(arg1) )            //if we actually added to the stack
                      [
                        statement.value += construct_<functor::Swapper>()   //swap so last is at top
                      ],
                      statement.value += construct_<functor::popper<1> >()  //pop last
                    ],
                    unwrap(statement.last) = second(arg1)                   //update definition of last
                  ]
              |   ifStatement
              |   returnStatement
              )
          >>  (   spirit::eol_p           //statement are normally ended by a newline
              |   spirit::eps_p( '}' )    //but an end of block would do too
              )
          ;

      stackingStatement
          =   expression( value::Expression(), statement.localTable, unwrap(statement.last), self.globalTable, self.functionTable )
              [
                statement.value += first(arg1),
                if_( second(arg1) == val('c') && ! third(arg1) )   //if type clip and not a top level =
                [                                                  //we define a last
                  second(stackingStatement.value) = bind(&VarTable::size)(unwrap(statement.localTable))
                ]
                .else_
                [
                  if_( second(arg1) != val('v') )                  //if expr is not type void
                  [
                    statement.value += construct_<popper<1> >()    //we pop it
                  ],
                  first(stackingStatement.value) = val(false)      //report we stack nothing
                ]
              ]
          |   createVar( false, statement.localTable )
          ;

      createVar
          =   (   spirit::str_p("global")
                  [
                    createVar.globalVar = val(true),
                    createVar.varTable = self.globalTable
                  ]
              |   !   spirit::str_p("local")
              )
          >>  name
              [
                createVar.name = construct_<std::string>(arg1, arg2)
              ]
          >>  '='
          >>  expression( value::Expression(), statement.localTable, unwrap(statement.last), self.globalTable, self.functionTable )
              [
                statement.value += first(arg1),     //accumulate creation code, then define var
                createVar.index = bind(&VarTable::DefineVar)( unwrap(createVar.varTable), createVar.name, second(arg1) ), 
                if_( createVar.globalVar )          //if global var
                [                                   //add appropriate assigning code
                  statement.value += construct_<popassigner<GlobalVar> >( createVar.index ),
                  first(stackingStatement.value) = val(false)     //report we leave nothing on stack
                ]
              ]
          ;

      ifStatement
          =   spirit::str_p("if")
          >>  '('
          >>  expression( value::Expression(), statement.localTable, unwrap(statement.last), self.globalTable, self.functionTable )
              [
                bind(&Check::TypeIsExpected)(second(arg1), val('b')),
                statement.value += first(arg1)
              ]
          >>  ')'
          >>  block( CodeCouple(), unwrap(statement.localTable) )
              [
                ifStatement.value = arg1
              ]          
          >>  (   *   spirit::eol_p
              >>  spirit::str_p("else")
              >>  block( CodeCouple(), unwrap(statement.localTable) )
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
          >>  '{'
          >> *(   statement( CodeCouple(), wrap(block.varTable), wrap(block.last) )
                  [
                    block.value += arg1
                  ]
              |   spirit::eol_p
              )
          >>  '}'
          ;

      returnStatement
          =   spirit::str_p("return")
          >>  expression( value::Expression(), statement.localTable, unwrap(statement.last), self.globalTable, self.functionTable )
              [
                bind(&Check::ReturnTypeIsExpected)(second(arg1), self.returnTypeExpected),
                statement.value += first(arg1),
                statement.value -= construct_<literal<OpType> >(RETURN)
              ]
          ;
    }

    spirit::rule<ScannerT> const & start() const { return top; }


  private:

    spirit::rule<ScannerT> top;
    spirit::rule<ScannerT, closure::InnerStatement::context_t> statement;
    spirit::rule<ScannerT, closure::Value<value::StackingStatement>::context_t> stackingStatement;
    spirit::rule<ScannerT, closure::CreateVar::context_t> createVar;
    spirit::rule<ScannerT, closure::Value<StatementCode>::context_t> ifStatement;
    spirit::rule<ScannerT, closure::StatementBlock::context_t> block;
    spirit::rule<ScannerT> returnStatement;

    Name name;
    Expression expression;

  };

};



} } } //namespace avs::parser::grammar

#endif //__AVS_PARSER_GRAMMAR_STATEMENT_H__
