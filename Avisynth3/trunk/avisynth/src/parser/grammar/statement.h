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
#include "../functor/if.h"
#include "../functor/popper.h"
#include "../functor/swapper.h"
#include "../lazy/add_symbol.h"


namespace avs { namespace parser { namespace grammar {



namespace value { 
  
typedef boost::tuples::tuple<boost::optional<int>, bool> StackingStatement;

}

namespace closure {


struct Statement : spirit::closure
    < Statement
    , VMCode
    , boost::reference_wrapper<VarTable>
    , boost::reference_wrapper<int>
    , boost::reference_wrapper<boost::optional<int> >
    >
{
  member1 value;
  member2 varTable;
  member3 stackSize;
  member4 last;
};


struct StatementBlock : spirit::closure
    < StatementBlock
    , VMCode
    , VarTable
    , int
    , boost::optional<int>
    >
{
  member1 value;
  member2 varTable;
  member3 stackSize;
  member4 last;
};


} //namespace closure




class Statement : public spirit::grammar<Statement, closure::Statement::context_t>
{

  FunctionTable const& functionTable;


public:  //structors

  Statement(FunctionTable const& _functionTable)
    : functionTable( _functionTable ) { }


public:  //definition nested class

  template <typename ScannerT>
  struct definition
  {

    definition(Statement const& self)
      : expression( self.functionTable )
    {

      using namespace lazy;
      using namespace phoenix;

      top
          =   statement( VMCode(), self.varTable, self.stackSize, self.last )
              [ 
                self.value = arg1 
              ]
          ;

      statement
          =  !(   stackingStatement( boost::tuples::make_tuple(boost::optional<int>(), true) )
                  [
                    if_( unwrap(statement.last) )   //if last was defined
                    [
                      if_( second(arg1) )           //if we actually added to the stack
                      [
                        statement.value += construct_<functor::Swapper>()   //swap so last is at top
                      ],
                      statement.value += construct_<functor::popper<1> >()  //pop last
                    ],
                    unwrap(statement.last) = first(arg1)                    //update definition of last
                  ]
              |   ifStatement
              )
          >>  spirit::eol_p
          ;

      stackingStatement
          =   expression( value::Expression(), statement.varTable, unwrap(statement.last) )
              [
                statement.value = first(arg1),
                if_( second(arg1) == val('c') && ! third(arg1) )          //if type clip and not a top level =
                [
                  stackingStatement.value = construct_<boost::optional<int> >( statement.stackSize )
                ]
                .else_
                [
                  if_( second(arg1) != val('v') )                         //if expr is not type void
                  [
                    statement.value += construct_<functor::popper<1> >()  //we pop it
                  ],
                  second(stackingStatement.value) = val(false)            //report we stack nothing
                ]
              ]
          |   createAssignVar
          ;

      createAssignVar
          =   name
              [
                createAssignVar.value = construct_<std::string>(arg1, arg2)
              ]
          >>  '='
          >>  expression( value::Expression(), statement.varTable, unwrap(statement.last) )
              [
                statement.value = first(arg1),
                add_symbol(  unwrap(statement.varTable), createAssignVar.value, 
                    construct_<TypedIndex>( statement.stackSize, second(arg1) )  ),
                unwrap(statement.stackSize) += val(1)
              ]
          ;

      ifStatement
          =   spirit::str_p("if")
          >>  '('
          >>  expression( value::Expression(), statement.varTable, unwrap(statement.last) )
              [
                statement.value = first(arg1)
              ]
          >>  ')'
          >>  *spirit::eol_p
          >>  block( VMCode(), unwrap(statement.varTable), unwrap(statement.stackSize) )
              [
                statement.value += construct_<functor::IfThen>(arg1)
              ]
          ;

      block
          =   '{'
          >> *(   statement( VMCode(), wrap(block.varTable), wrap(block.stackSize), wrap(block.last) )
                  [
                    block.value += arg1
                  ]
              )
          >>  spirit::ch_p('}')
              [
                block.value += construct_<functor::Popper>( block.stackSize - unwrap(statement.stackSize) )
              ]
          ;

    }

    spirit::rule<ScannerT> const & start() const { return top; }


  private:

    spirit::rule<ScannerT> top;
    spirit::rule<ScannerT, closure::Statement::context_t> statement;
    spirit::rule<ScannerT, closure::Value<value::StackingStatement>::context_t> stackingStatement;
    spirit::rule<ScannerT, closure::Value<std::string>::context_t> createAssignVar;
    spirit::rule<ScannerT> ifStatement;
    spirit::rule<ScannerT, closure::StatementBlock::context_t> block;

    Name name;
    Expression expression;

  };

};



} } } //namespace avs::parser::grammar

#endif //__AVS_PARSER_GRAMMAR_STATEMENT_H__
