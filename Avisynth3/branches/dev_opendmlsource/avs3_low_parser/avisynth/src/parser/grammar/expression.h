// Avisynth v3.0 alpha.  Copyright 2003-2006 David Pierre - Ben Rudiak-Gould et al.
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
#include "../function/forward.h"               //for OperatorTable declaration
//#include "../vmcode.h"
//#include "../function/table.h"

//boost includes
//#include <boost/optional.hpp>               //for optional


namespace avs { namespace parser { namespace grammar {


/*
namespace closures {



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


////////////////////////////////////////////////////////////////////////////////////////////////
//  closure::FunctionCall
//
//  closure for the function call rule with the Expression grammar
//
struct FunctionCall : spirit::closure
    < FunctionCall
    , std::string
    , function::Pool const *
    >
{
  member1 prototype;
  member2 functionPool;
};


} //namespace closures


*/

///////////////////////////////////////////////////////////////////////////////////////
//  grammar::Expression
//
//  expression grammar
//
class Expression : public spirit::grammar<Expression, closures::Value<char>::context_t>
{
/*
  spirit::symbols<bool> equality_op;                //maps "==" to true and "!=" to false
  spirit::symbols<char const> add_op, mult_op;      //maps the symbol to itself
*/

  Literal literal_;
  std::ostream& out_;
  function::OperatorTable const& opTable_;


public:  //structors

  Expression(std::ostream& out, function::OperatorTable const& opTable);


public:  //definition nested class

  template <typename ScannerT>
  struct definition 
  {
        
    definition(Expression const& self);


    spirit::rule<ScannerT> const& start() const { return expression; }


  private:

    //spirit::rule<ScannerT> top;
    spirit::rule<ScannerT, closures::Value<char>::context_t> expression;
/*    spirit::rule<ScannerT, closures::Value<TypedIndex>::context_t> local_assign_expr;
    spirit::rule<ScannerT, closures::Value<TypedIndex>::context_t> global_assign_expr;
    spirit::rule<ScannerT, closures::Value<bool>::context_t> equality_expr;
*/
    spirit::rule<ScannerT, closures::Value<char>::context_t> add_expr;
/*
    spirit::rule<ScannerT, closures::Value<char>::context_t> mult_expr;
    spirit::rule<ScannerT, closures::Value<value::ElementalOpProxy>::context_t> binaryop_helper;
*/
    spirit::rule<ScannerT, closures::Value<char>::context_t> atom_expr;
/*
    spirit::rule<ScannerT> nested_expr, global_var_expr;
    spirit::rule<ScannerT, closures::Value<int>::context_t> local_var_expr;
    spirit::rule<ScannerT, closures::FunctionCall::context_t> call_expr;
    spirit::rule<ScannerT> subscript_helper;
    spirit::rule<ScannerT, closures::Value<std::string>::context_t> infix_helper;
    spirit::rule<ScannerT, closures::Value<char>::context_t> fixed_type_expr;

    Literal literal;
*/
  };

};



template <>
Expression::definition<Scanner>::definition(Expression const & self);



} } } //namespace avs::parser::grammar

#endif //__AVS_PARSER_GRAMMAR_EXPRESSION_H__
