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


#ifndef __AVS_PARSER_GRAMMAR_LITERAL_H__
#define __AVS_PARSER_GRAMMAR_LITERAL_H__

//avisynth includes
#include "../stack.h"
#include "../lazy/pair.h"
#include "../stackoperation.h"

//spirit include
#include <boost/spirit/core.hpp>
#include <boost/spirit/attribute/closure.hpp>
#include <boost/spirit/phoenix/statements.hpp>      //for operator,
//#include <boost/spirit/utility/escape_char.hpp>   //for c_escape_ch_p, the day it starts converting

namespace spirit = boost::spirit;


namespace avs { namespace parser { namespace grammar {
  
  

namespace closure {


template <class Type> struct Value : spirit::closure<Value, Type>
{
  member1 value;
};


} //namespace closure
 




struct Literal : public spirit::grammar<Literal, closure::Value<TypedOp>::context_t>
{

  //stack_pusher template functor
  template <class Value>
  struct stack_pusher
  {

    Value const value_;

    stack_pusher(Value const& value)
      : value_( value ) { }

    void operator()(Stack& stack) const { stack.push( value_ ); }

  };


  template <typename ScannerT>
  struct definition 
  {
        
    definition(Literal const& self)
    {

      using namespace lazy;
      using namespace phoenix;

      literal
          =   spirit::strict_real_p
              [
                first(self.value) = construct_<stack_pusher<double> >(arg1),
                second(self.value) = val('d')
              ]
          |   spirit::int_p
              [
                first(self.value) = construct_<stack_pusher<int> >(arg1),
                second(self.value) = val('i')
              ]
          |   string
              [
                first(self.value) = construct_<stack_pusher<std::string> >(arg1),
                second(self.value) = val('s')
              ]
          ;

      string
          =   spirit::lexeme_d
              [
                  '"'
              >> *(   spirit::str_p("\\n")
                      [
                        string.value += val('\n')
                      ]
                  |   ( spirit::anychar_p - '"' )
                      [
                        string.value += *arg1
                      ]
                  )
              >>  '"'
              ]
          ;
    }

    spirit::rule<ScannerT> const & start() const { return literal; }


  private:

    spirit::rule<ScannerT> literal;
    spirit::rule<ScannerT, closure::Value<std::string>::context_t> string;

  };

};


} } } //namespace avs::parser::grammar

#endif //__AVS_PARSER_GRAMMAR_LITERAL_H__
