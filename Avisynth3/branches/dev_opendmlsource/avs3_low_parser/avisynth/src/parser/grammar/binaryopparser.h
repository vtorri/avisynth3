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


#ifndef __AVS_PARSER_GRAMMAR_BINARYOPPARSER_H__
#define __AVS_PARSER_GRAMMAR_BINARYOPPARSER_H__

//avisynth includes
//#include "../action/get.h"
#include "../function/forward.h"          //for OperatorTable

//STL include
#include <iosfwd>

//boost includes
#include <boost/spirit/core.hpp>
#include <boost/spirit/phoenix.hpp>
#include <boost/spirit/utility/functor_parser.hpp>

namespace spirit = boost::spirit;


namespace avs { namespace parser { namespace grammar { namespace binaryop {




template <typename TermT>
class parser
{

  std::ostream& out_;
  char const op1_;
  char const op2_;

  function::OperatorTable const& opTable_;
  typename spirit::as_parser<TermT>::type::embed_t term_;


public:  //constructor

  parser(std::ostream& out, function::OperatorTable const& opTable, TermT const & term, char op1, char op2)
    : out_( out ) 
    , op1_( op1 )
    , op2_( op2 )
    , opTable_( opTable )
    , term_( term ) { }


public:  //parser interface

  typedef char result_t;

  template <typename ScannerT>
  std::ptrdiff_t operator()(ScannerT const& scan, result_t& result) const;

/*
  {

    char opSymbol;
    char rightType;

    using namespace phoenix;

    typename spirit::match_result<ScannerT, result_t>::type match
        = (   term_
              [
                var(result) = arg1
              ]
          >> *(   op_table_
                  [
                    var(opSymbol) = arg1
                  ]
              >>  term_
                  [
                    var(rightType) = arg1
                  ]
              >>  act_( bind(&action::Get::BinaryOperation)(var(result), var(rightType), var(opSymbol)) )
              )                              
          ).parse(scan);

    return match.length();
  }
*/
};


struct parser_gen
{
  template <typename TermT>
  spirit::functor_parser<
	    parser<
          typename spirit::as_parser<TermT>::type
      >
  >
  operator()(std::ostream& out, function::OperatorTable const& opTable, TermT const & term, char op1, char op2) const
  {
  
    typedef typename spirit::as_parser<TermT>::type term_t;

    typedef parser<term_t> functor_t;
    typedef spirit::functor_parser<functor_t> return_t;

    return return_t(
        functor_t(
            out,
            opTable,
            spirit::as_parser<TermT>::convert(term),
            op1,
            op2
        )
    );
  }

};


} //namespace binaryop


//binary op parser generator used in the expression grammar
binaryop::parser_gen const binary_op_p = binaryop::parser_gen();


} } } //namespace avs::parser::grammar

#endif //__AVS_PARSER_GRAMMAR_BINARYOPPARSER_H__
