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


#ifndef __AVS_PARSER_GRAMMAR_LITERAL_H__
#define __AVS_PARSER_GRAMMAR_LITERAL_H__

//avisynth includes
#include "forward.h"                  //for value::Literal
//#include "../forward.h"
#include "../vmoperation.h"

//spirit includes
#define PHOENIX_LIMIT 6
#include <boost/spirit/core/non_terminal/rule.hpp>
#include <boost/spirit/core/non_terminal/grammar.hpp>
#include <boost/spirit/attribute/closure.hpp>          //grammar.hpp before that one


namespace spirit = boost::spirit;


namespace avs { namespace parser { namespace grammar {
  


namespace closure {


template <class Type> struct Value : spirit::closure<Value<Type>, Type>
{
  typename spirit::closure<Value<Type>, Type>::member1 value;
};


} //namespace closure
 


////////////////////////////////////////////////////////////////////////////////////////////
//  grammar::Literal
//
//  grammar to recognize literals
//
struct Literal : public spirit::grammar<Literal, closure::Value<value::Literal>::context_t>
{

  template <typename ScannerT>
  struct definition 
  {
        
    definition(Literal const& self);

    spirit::rule<ScannerT> const & start() const { return top; }


  private:

    spirit::rule<ScannerT> top;
    spirit::rule<ScannerT, closure::Value<std::string>::context_t> stringLiteral;

  };

};

typedef spirit::scanner< char const *
                       , spirit::scanner_policies
                           < spirit::skip_parser_iteration_policy
                               < spirit::blank_parser
                               , spirit::iteration_policy
                               >
                           , spirit::match_policy
                           , spirit::action_policy
                           > 
                       > 
        Scanner;


template <>
Literal::definition<Scanner>::definition(Literal const & self);



} } } //namespace avs::parser::grammar

#endif //__AVS_PARSER_GRAMMAR_LITERAL_H__
