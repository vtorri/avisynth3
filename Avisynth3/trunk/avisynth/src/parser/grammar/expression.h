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


#ifndef __AVS_PARSER_GRAMMAR_EXPRESSION_H__
#define __AVS_PARSER_GRAMMAR_EXPRESSION_H__

//avisynth includes
#include "get.h"
#include "check.h"
#include "literal.h"
#include "forward.h"
#include "../vmcode.h"
#include "../lazy/ref.h"
#include "../function/table.h"
#include "../binaryop/parser.h"
#include "../functor/var.h"
#include "../functor/assigner.h"

//boost includes
#include <boost/optional.hpp>               //for optional
#include <boost/spirit/dynamic/lazy.hpp>    //for lazy_p


namespace avs { namespace parser { namespace grammar {



namespace closure {



//////////////////////////////////////////////////////////////////////////////////////////////
//  closure::Expression
//
//  closure for the Expression grammar
//
struct Expression : spirit::closure
    < Expression
    , value::Expression
    , boost::reference_wrapper<LocalContext>
    , boost::reference_wrapper<GlobalContext>
    >
{
  member1 value;
  member2 localCtxt;
  member3 globalCtxt;
};


////////////////////////////////////////////////////////////////////////////////////////////////
//  closure::InnerExpression
//
//  closure for the expression rule within the Expression grammar
//
struct InnerExpression : spirit::closure
    < InnerExpression
    , char
    , bool
    >
{
  member1 type;
  member2 implicit;   //mark if implicit last function call are possible within the expression
};

struct FunctionCall : spirit::closure<FunctionCall, std::string, function::Pool const *>
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

  spirit::symbols<bool> equality_op;                     //maps "==" to true and "!=" to false
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
          =   expression( char(), third(self.localCtxt) )   //init expression.implicit to true if last defined
              [ 
                second(self.value) = arg1                   //set expr type in the grammar closure
              ]
          ;

      expression 
          =   (   local_assign_expr   |   global_assign_expr    )
              [ 
                third(self.value) = val(true)      //report the expression parsed is an affectation
              ]
          |   equality_expr
              [ 
                third(self.value) = val(false)     //report it is not
              ]
          ;

      local_assign_expr
          =   spirit::lazy_p( first(self.localCtxt) )   //use local VarTable (1st member of localCtxt)
              [ 
                local_assign_expr.value = arg1 
              ]
          >>  '='
          >>  expression( char(), expression.implicit )
              [
                expression.type = arg1,
                first(self.value) += construct_<LocalVarAssigner>( second(self.localCtxt) - first(local_assign_expr.value) )
              ]
          ;

      global_assign_expr
          =   spirit::str_p("global")
          >>  spirit::lazy_p( first(self.globalCtxt) )   //use global VarTable (1st member of globalCtxt)
              [
                global_assign_expr.value = arg1
              ]
          >>  '='
          >>  expression( char(), expression.implicit )
              [
                expression.type = arg1,
                first(self.value) += construct_<GlobalVarAssigner>( first(global_assign_expr.value) )
              ]
          ;

      equality_expr
          =   add_expr
              [
                expression.type = arg1
              ]
          >> !(   self.equality_op
                  [
                    equality_expr.value = arg1   //true if ==, else false
                  ]
              >>  add_expr
                  [
                    first(self.value) += bind(&Get::EqualityOperation)(expression.type, arg1, equality_expr.value),
                    --second(self.localCtxt)
                  ]
              )
          ;

      add_expr
          =   binary_op_p( mult_expr, self.add_op, binaryop_helper )
              [
                add_expr.value = arg1
              ]
          ;

      mult_expr
          =   binary_op_p( atom_expr, self.mult_op, binaryop_helper )
              [
                mult_expr.value = arg1
              ]
          ;

      binaryop_helper
          =   spirit::eps_p 
              [
                first(self.value) += first(binaryop_helper.value),
                --second(self.localCtxt)
              ]
          ;

      atom_expr                        //atom can be
          =   (   nested_expr          //a nested expr (ie ( expr ) )
              |   call_expr            //a function call
              |   (   literal          //a literal
                      [ 
                        first(self.value) += first(arg1),   //accumulate code
                        atom_expr.value = second(arg1)      //update type
                      ]
                  |   local_var_expr   //a local variable (last counting as one)
                      [
                        first(self.value) += construct_<LocalVarPusher>( second(self.localCtxt) - arg1 )
                      ]
                  |   global_var_expr  //a global variable
                  )
                  [
                    ++second(self.localCtxt)       //these 3 don't update stack size themselves
                  ]
              )
              [
                expression.implicit = val(false)   //deactivate implicit last for the following
              ]
          >> *(   subscript_helper( false )
              ||  infix_helper
              )
         ;

      nested_expr
          =   '('
          >>  expression( char(), false )
              [
                atom_expr.value = arg1
              ]
          >>  ')'
          ;

      local_var_expr
          =   spirit::lazy_p( first(self.localCtxt) )    //use local VarTable (1st member of localCtxt)
              [
                local_var_expr.value = first(arg1),
                atom_expr.value = second(arg1)
              ]
          |   spirit::str_p("last")
              [
                local_var_expr.value = bind(&Check::LastIsDefined)(third(self.localCtxt)),
                atom_expr.value = val('c')
              ]
          ;

      global_var_expr
          =   !   spirit::str_p("global")
          >>  spirit::lazy_p( first(self.globalCtxt) )   //use global VarTable (1st member of globalCtxt)
              [
                first(self.value) += construct_<GlobalVarPusher>( first(arg1) ),
                atom_expr.value = second(arg1)
              ]
          ;

      call_expr
          =   spirit::lazy_p( second(self.globalCtxt) )  //use the functionTable  (2nd member of globalCtxt)
              [
                call_expr.functionPool = bind(&boost::reference_wrapper<function::Pool>::get_pointer)(arg1)
              ]
          >>  '('
          >> !(   expression( char(), false )
                  [
                    call_expr.prototype += arg1
                  ]
              %   ','
              )
          >>  spirit::ch_p(')')
              [
                atom_expr.value = bind(&function::Pool::Resolve)(call_expr.functionPool, call_expr.prototype, first(self.value)),
                second(self.localCtxt) -= bind(&std::string::size)(call_expr.prototype),
                if_( atom_expr.value != val('v') )           //if don't return void
                [
                  ++second(self.localCtxt)                   //take return value into account for stack size
                ]
              ]
          ;

      subscript_helper
          =  '['
          >>  expression( char(), false )
              [
                bind(&Check::TypeIsExpected)(arg1, val('i'))
              ]
          >>  ','
          >>  (   spirit::str_p("end")
                  [
                    subscript_helper.value = val(true),
                    --second(self.localCtxt)
                  ]
              |   expression( char(), false )
                  [
                    bind(&Check::TypeIsExpected)(arg1, val('i')),
                    second(self.localCtxt) -= 2
                  ]
              )
          >>  spirit::ch_p(']')
              [
                first(self.value) += bind(&Get::SubscriptOperation)(atom_expr.value, subscript_helper.value)
              ]
          ;


      infix_helper
          =   spirit::ch_p('.')
              [   //check if infix allowed (throw if not)                
                infix_helper.value = bind(&Check::IsInfixable)(atom_expr.value)
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
    spirit::rule<ScannerT, closure::Value<bool>::context_t> equality_expr;
    spirit::rule<ScannerT, closure::Value<char>::context_t> add_expr;
    spirit::rule<ScannerT, closure::Value<char>::context_t> mult_expr;
    spirit::rule<ScannerT, closure::Value<value::ElementalOpProxy>::context_t> binaryop_helper;
    spirit::rule<ScannerT, closure::Value<char>::context_t> atom_expr;
    spirit::rule<ScannerT> nested_expr, global_var_expr;
    spirit::rule<ScannerT, closure::Value<int>::context_t> local_var_expr;
    spirit::rule<ScannerT, closure::FunctionCall::context_t> call_expr;
    spirit::rule<ScannerT, closure::Value<bool>::context_t> subscript_helper;
    spirit::rule<ScannerT, closure::Value<std::string>::context_t> infix_helper;

    Literal literal;

  };

};


} } } //namespace avs::parser::grammar

#endif //__AVS_PARSER_GRAMMAR_EXPRESSION_H__
