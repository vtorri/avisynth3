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


#ifndef __AVS_PARSER_GRAMMAR_BINARYOPPARSER_H__
#define __AVS_PARSER_GRAMMAR_BINARYOPPARSER_H__

//avisynth includes
#include "../action/get.h"

//boost includes
#include <boost/spirit/core.hpp>
#include <boost/spirit/phoenix.hpp>
#include <boost/spirit/utility/functor_parser.hpp>

namespace spirit = boost::spirit;


namespace avs { namespace parser { namespace grammar { namespace binaryop {




template <typename TermT, typename OpT, typename ActT>
class parser
{

  typename spirit::as_parser<TermT>::type::embed_t term_;
  typename spirit::as_parser<OpT>::type::embed_t op_table_;
  typename spirit::as_parser<ActT>::type::embed_t act_;


public:  //constructor

  parser(TermT const & term, OpT const & op_table, ActT const & act)
    : term_( term ) 
    , op_table_( op_table )
    , act_( act ) { }


public:  //parser interface

  typedef char result_t;

  template <typename ScannerT>
  std::ptrdiff_t operator()(ScannerT const& scan, result_t& result) const
  {

    char opSymbol;
    char rightType;

    using namespace lazy;
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

};


struct parser_gen
{
  template <typename TermT, typename OpT, typename ActT>
  spirit::functor_parser<
	    parser<
          typename spirit::as_parser<TermT>::type,
          typename spirit::as_parser<OpT>::type,
          typename spirit::as_parser<ActT>::type
      >
  >
  operator()(TermT const & term, OpT const & op_table, ActT const & act) const
  {
  
    typedef typename spirit::as_parser<TermT>::type term_t;
    typedef typename spirit::as_parser<OpT>::type op_t;
    typedef typename spirit::as_parser<ActT>::type act_t;
    typedef parser<term_t, op_t, act_t> functor_t;
    typedef spirit::functor_parser<functor_t> return_t;

    return return_t(
        functor_t(
            spirit::as_parser<TermT>::convert(term),
            spirit::as_parser<OpT>::convert(op_table),
            spirit::as_parser<ActT>::convert(act)
        )
    );
  }

};


} //namespace binaryop


//binary op parser generator used in the expression grammar
binaryop::parser_gen const binary_op_p = binaryop::parser_gen();


} } } //namespace avs::parser::grammar

#endif //__AVS_PARSER_GRAMMAR_BINARYOPPARSER_H__
