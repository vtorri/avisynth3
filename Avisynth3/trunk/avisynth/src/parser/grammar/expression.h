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


#ifndef __AVS_PARSER_GRAMMAR_EXPRESSION_H__
#define __AVS_PARSER_GRAMMAR_EXPRESSION_H__

//avisynth includes
#include "check.h"
#include "action.h"
#include "literal.h"
#include "vartable.h"
#include "../vmcode.h"
#include "../lazy/unwrap.h"
#include "../functiontable.h"
#include "../binaryop/parser.h"
#include "../functor/var.h"
#include "../functor/assigner.h"

//boost includes
#include <boost/optional.hpp>               //for optional
#include <boost/spirit/dynamic/lazy.hpp>    //for lazy_p


namespace avs { namespace parser { namespace grammar {


//typedef
namespace value { typedef boost::tuples::tuple<ElementalCode, char, bool> Expression; }


namespace closure {



//////////////////////////////////////////////////////////////////////////////////////////////
//  closure::Expression
//
//  closure for the Expression grammar
//
struct Expression : spirit::closure
    < Expression
    , value::Expression
    , boost::reference_wrapper<VarTable>
    , boost::optional<int>
    , boost::reference_wrapper<VarTable>
    , boost::reference_wrapper<FunctionTable>
    >
{
  member1 value;
  member2 localTable;
  member3 last;
  member4 globalTable;
  member5 functionTable;
};


////////////////////////////////////////////////////////////////////////////////////////////////
//  closure::InnerExpression
//
//  closure for the expression rule within the Expression grammar
//
struct InnerExpression : spirit::closure
    < InnerExpression
    , TypedCode
    , bool
    >
{
  member1 value;
  member2 implicit;   //mark if implicit last function call are possible within the expression
};

struct FunctionCall : spirit::closure<FunctionCall, std::string, FunctionPool const *>
{
  member1 prototype;
  member2 functionPool;
};


} //namespace closure




///////////////////////////////////////////////////////////////////////////////////////
//  grammar::Expression
//
//  expression grammar
//
class Expression : public spirit::grammar<Expression, closure::Expression::context_t>
{

  spirit::symbols<binaryop::TypeMapped const> add_op, mult_op;


public:  //structors

  Expression();


private:  //typedefs 

  typedef functor::pusher<functor::LocalVar> LocalVarPusher;
  typedef functor::pusher<functor::GlobalVar> GlobalVarPusher;
  typedef functor::assigner<functor::LocalVar> LocalVarAssigner;
  typedef functor::assigner<functor::GlobalVar> GlobalVarAssigner;


public:  //definition nested class

  template <typename ScannerT>
  struct definition 
  {
        
    definition(Expression const& self)
    {

      using namespace lazy;
      using namespace phoenix;

      top 
          =   expression( TypedCode(), self.last() ) 
              [ 
                first(self.value) = first(arg1),
                second(self.value) = second(arg1)
              ]
          ;

      expression 
          =   (   local_assign_expr   |   global_assign_expr    )
              [ 
                third(self.value) = val(true)      //report the expression parsed is an affectation
              ]
          |   binary_op_p(mult_expr, self.add_op) 
              [ 
                expression.value = arg1,
                third(self.value) = val(false)     //report it is not
              ]
          ;

      local_assign_expr
          =   spirit::lazy_p( unwrap(self.localTable) )
              [ 
                local_assign_expr.value = arg1 
              ]
          >>  '='
          >>  expression( TypedCode(), expression.implicit )
              [
                expression.value = arg1,
                first(expression.value) += construct_<LocalVarAssigner>( first(local_assign_expr.value) )
              ]
          ;

      global_assign_expr
          =   spirit::str_p("global")
          >>  spirit::lazy_p( unwrap( self.globalTable) )
              [
                global_assign_expr.value = arg1
              ]
          >>  '='
          >>  expression( TypedCode(), expression.implicit )
              [
                expression.value = arg1,
                first(expression.value) += construct_<GlobalVarAssigner>( first(global_assign_expr.value) )
              ]
          ;

      mult_expr
          =   binary_op_p(atom_expr, self.mult_op) [ mult_expr.value = arg1 ];

      atom_expr                      //atom can be
          =   (   literal            //a literal
                  [ 
                    atom_expr.value = construct_<TypedCode>(first(arg1), second(arg1)) 
                  ]
              |   nested_expr        //a nested expr (ie ( expr ) )
              |   call_expr          //a function call
              |   var_expr           //a variable
              |   last_expr          //an explicit last
              )
              [
                expression.implicit = val(false)
              ]
          >> *(   subscript_helper( false )
              ||  infix_helper
              )
         ;

      nested_expr
          =   '('
          >>  expression( TypedCode(), false )
              [
                atom_expr.value = arg1
              ]
          >>  ')'
          ;

      var_expr
          =   spirit::lazy_p( unwrap(self.localTable) )
              [
                first(atom_expr.value) += construct_<LocalVarPusher>( first(arg1) ),
                second(atom_expr.value) = second(arg1)
              ]
          |   !   spirit::str_p("global")
          >>  spirit::lazy_p( unwrap(self.globalTable) )
              [
                first(atom_expr.value) += construct_<GlobalVarPusher>( first(arg1) ),
                second(atom_expr.value) = second(arg1)
              ]
          ;

      last_expr
          =   spirit::str_p("last")
              [
                first(atom_expr.value) += construct_<LocalVarPusher>( bind(&Check::LastIsDefined)( self.last ) ),
                second(atom_expr.value) = val('c')
              ]
          ;

      call_expr
          =   spirit::lazy_p( unwrap(self.functionTable) )
              [
                call_expr.functionPool = bind(&boost::reference_wrapper<FunctionPool>::get_pointer)(arg1)
              ]
          >>  '('
          >> !(   expression( TypedCode(), false )
                  [
                    first(atom_expr.value) += first(arg1),
                    call_expr.prototype += second(arg1)
                  ]
              %   ','
              )
          >>  spirit::ch_p(')')
              [
                bind(&FunctionPool::Resolve)(call_expr.functionPool, call_expr.prototype, atom_expr.value)
              ]
          ;

      subscript_helper
          =  '['
          >>  int_expr_helper
          >>  ','
          >>  (   spirit::str_p("end")
                  [
                    subscript_helper.value = val(true)
                  ]
              |   int_expr_helper
              )
          >>  spirit::ch_p(']')
              [
                bind(&Action::AppendSubscriptOperation)(atom_expr.value, subscript_helper.value)
              ]
          ;

      int_expr_helper
          =   expression( TypedCode(), false )
              [
                bind(&Check::TypeIsExpected)(second(arg1), val('i')),
                first(atom_expr.value) += first(arg1)
              ]
          ;

      infix_helper
          =   spirit::ch_p('.')
              [   //check if infix allowed (throw if not)                
                infix_helper.value = bind(&Check::IsInfixable)(second(atom_expr.value))
              ]
              >>  call_expr( infix_helper.value )
          ;
 
    }

    spirit::rule<ScannerT> const& start() const { return top; }


  private:

    spirit::rule<ScannerT> top;
    spirit::rule<ScannerT, closure::InnerExpression::context_t> expression;
    spirit::rule<ScannerT, closure::Value<TypedIndex>::context_t> local_assign_expr;
    spirit::rule<ScannerT, closure::Value<TypedIndex>::context_t> global_assign_expr;
    spirit::rule<ScannerT, closure::Value<TypedCode>::context_t> mult_expr;
    spirit::rule<ScannerT, closure::Value<TypedCode>::context_t> atom_expr;
    spirit::rule<ScannerT> nested_expr, var_expr, last_expr;
    spirit::rule<ScannerT, closure::FunctionCall::context_t> call_expr;
    spirit::rule<ScannerT, closure::Value<bool>::context_t> subscript_helper;
    spirit::rule<ScannerT> int_expr_helper;
    spirit::rule<ScannerT, closure::Value<std::string>::context_t> infix_helper;

    Literal literal;

  };

};


} } } //namespace avs::parser::grammar

#endif //__AVS_PARSER_GRAMMAR_EXPRESSION_H__
