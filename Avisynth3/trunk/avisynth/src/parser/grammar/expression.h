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
#include "../lazy/pair.h"
#include "../functionpool.h"
#include "../binaryop/parser.h"

//boost include
#include <boost/spirit/symbols.hpp>

//stl include
#include <utility>   //for pair


namespace avs { namespace parser { namespace grammar {


//typedefs
typedef std::pair<int, char> TypedIndex;
typedef spirit::symbols<TypedIndex const> VarTable;
typedef spirit::symbols<FunctionPool> FunctionTable;


namespace closure {


struct FunctionCall : spirit::closure<FunctionCall, phoenix::nil_t, FunctionPool const *, std::string>
{
  member1 dummy;
  member2 functionPool;
  member3 prototype;
};


} //namespace closure




class Expression : public spirit::grammar<Expression, closure::Value<TypedCode>::context_t>
{

  VarTable const& varTable;
  FunctionTable const& functionTable;
  spirit::symbols<binaryop::TypeMapped const> add_op, mult_op;


public:  //structors

  Expression(VarTable const& _varTable, FunctionTable const& _functionTable);



private:  //inner classes


  //var_pusher functor
  struct var_pusher
  {

    int index_;

    var_pusher(int index)
      : index_( index ) { }

    void operator()(VMState& state) const { state.push(state[index_]); }

  };

  //var_assigner functor
  struct var_assigner
  {

    int index_;

    var_assigner(int index)
      : index_( index ) { }

    void operator()(VMState& state) const { state[index_] = state.top(); }

  };


public:  //definition nested class

  template <typename ScannerT>
  struct definition 
  {
        
    definition(Expression const& self)
    {

      using namespace lazy;
      using namespace phoenix;

      top = expression [ self.value = arg1 ];

      expression 
          =   binary_op_p(mult_expr, self.add_op) [ expression.value = arg1 ];

      mult_expr
          =   binary_op_p(atom_expr, self.mult_op) [ mult_expr.value = arg1 ];

      atom_expr
          =   literal [ atom_expr.value = construct_<TypedCode>(first(arg1), second(arg1)) ]
          |   '('
          >>  expression [ atom_expr.value = arg1 ]
          >>  ')'
          |   var_expr
          ;

      var_expr
          =   self.varTable [ var_expr.value = arg1 ]
          >>  (   '='
              >>  expression
                  [
                    atom_expr.value = arg1,
                    first(atom_expr.value) += construct_<var_assigner>( first(var_expr.value) )
                  ]
              |   spirit::epsilon_p
                  [
                    first(atom_expr.value) += construct_<var_pusher>( first(var_expr.value) ),
                    second(atom_expr.value) = second(var_expr.value)
                  ]
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
    spirit::rule<ScannerT, closure::Value<TypedCode>::context_t> mult_expr;
    spirit::rule<ScannerT, closure::Value<TypedCode>::context_t> atom_expr;
    spirit::rule<ScannerT, closure::Value<TypedIndex>::context_t> var_expr;
    spirit::rule<ScannerT, closure::FunctionCall::context_t> call_expr;

    Literal literal;

  };

};


} } } //namespace avs::parser::grammar

#endif //__AVS_PARSER_GRAMMAR_EXPRESSION_H__