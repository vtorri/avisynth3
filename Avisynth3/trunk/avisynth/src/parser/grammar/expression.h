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
#include "literal.h"
#include "../vmcode.h"
#include "../lazy/tuple.h"
#include "../functiontable.h"
#include "../binaryop/parser.h"
#include "../functor/pusher.h"
#include "../functor/assigner.h"
#include "../functor/localvar.h"

//boost includes
#include <boost/optional.hpp>
#include <boost/spirit/symbols.hpp>

//stl include
#include <utility>   //for pair


namespace avs { namespace parser { namespace grammar {


//typedefs
typedef boost::tuples::tuple<int, char> TypedIndex;
typedef spirit::symbols<TypedIndex const> VarTable;


namespace closure {


//////////////////////////////////////////////////////////////////////////////////////////////
//  closure::Expression
//
//  closure for the Expression grammar
//
struct Expression : spirit::closure<Expression, boost::tuples::tuple<VMCode, char, bool>, boost::optional<int>, bool>
{
  member1 value;
  member2 last;
  member3 implicit;   //mark if implicit last function call are possible
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

  VarTable const& varTable;
  FunctionTable const& functionTable;
  spirit::symbols<binaryop::TypeMapped const> add_op, mult_op;


public:  //structors

  Expression(VarTable const& _varTable, FunctionTable const& _functionTable);


private:  //typedefs 

  typedef functor::pusher<functor::LocalVar> LocalVarPusher;
  typedef functor::assigner<functor::LocalVar> LocalVarAssigner;


public:  //definition nested class

  template <typename ScannerT>
  struct definition 
  {
        
    definition(Expression const& self)
    {

      using namespace lazy;
      using namespace phoenix;

      top 
          =   spirit::eps_p
              [
                self.implicit = !! self.last       //init if implicit last is available
              ]
          >>  expression 
              [ 
                first(self.value) = first(arg1),
                second(self.value) = second(arg1)
              ]
          ;

      expression 
          =   assign_expr 
              [ 
                third(self.value) = val(true)
              ]
          |   binary_op_p(mult_expr, self.add_op) 
              [ 
                expression.value = arg1,
                third(self.value) = val(false)
              ]
          ;

      assign_expr
          =   self.varTable [ assign_expr.value = arg1 ]
          >>  '='
          >>  expression
              [
                expression.value = arg1,
                first(expression.value) += construct_<LocalVarAssigner>( first(assign_expr.value) )
              ]
          ;

      mult_expr
          =   binary_op_p(atom_expr, self.mult_op) [ mult_expr.value = arg1 ];

      atom_expr
          =   (   literal [ atom_expr.value = construct_<TypedCode>(first(arg1), second(arg1)) ]
              |   '('
              >>  expression [ atom_expr.value = arg1 ]
              >>  ')'
              |   call_expr
              |   self.varTable
                  [
                    first(atom_expr.value) += construct_<LocalVarPusher>( first(arg1) ),
                    second(atom_expr.value) = second(arg1)
                  ]
              |   spirit::eps_p( !! self.last )
              >>  spirit::str_p("last")
                  [
                    first(atom_expr.value) += construct_<LocalVarPusher>( bind(&boost::optional<int>::get)(self.last) ),
                    second(atom_expr.value) = val('c')
                  ]
              )
              [
                self.implicit = val(false)
              ]
          >> *(   spirit::eps_p( second(atom_expr.value) == val('c') )  //check we have a clip result 
              >>  '.'
              >>  call_expr( "c" )
              )  
          ;

      call_expr
          =   self.functionTable
              [
                call_expr.functionPool = bind(&boost::reference_wrapper<FunctionPool>::get_pointer)(arg1)
              ]
          >>  '('
          >> !(   expression
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
    }

    spirit::rule<ScannerT> const& start() const { return top; }


  private:

    spirit::rule<ScannerT> top;
    spirit::rule<ScannerT, closure::Value<TypedCode>::context_t> expression;
    spirit::rule<ScannerT, closure::Value<TypedIndex>::context_t> assign_expr;
    spirit::rule<ScannerT, closure::Value<TypedCode>::context_t> mult_expr;
    spirit::rule<ScannerT, closure::Value<TypedCode>::context_t> atom_expr;
    spirit::rule<ScannerT, closure::FunctionCall::context_t> call_expr;

    Literal literal;

  };

};


} } } //namespace avs::parser::grammar

#endif //__AVS_PARSER_GRAMMAR_EXPRESSION_H__
