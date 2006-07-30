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


#ifndef __AVS_PARSER_GRAMMAR_SCRIPT_H__
#define __AVS_PARSER_GRAMMAR_SCRIPT_H__

//avisynth include
#include "statement.h"


namespace avs { namespace parser { namespace grammar {



namespace closures {


struct Script : spirit::closure
    < Script
    , CodeCouple
    , boost::reference_wrapper<LocalContext>
    , boost::reference_wrapper<GlobalContext>
    >
{
  member1 value;
  member2 localCtxt;
  member3 globalCtxt;
};


struct Function : spirit::closure
    < Function
    , function::FunctionId
    , LocalContext
    , CodeCouple
    , bool
    >
{
  member1 ident;
  member2 localCtxt;
  member3 code;
  member4 termRecursive;
};




} //namespace closures



struct Script : public spirit::grammar<Script, closures::Script::context_t>
{

  template <typename ScannerT>
  struct definition
  {

    definition(Script const& self);

    spirit::rule<ScannerT> const & start() const { return top; }


  private:

    spirit::rule<ScannerT> top;
    spirit::rule<ScannerT, closures::Function::context_t> function;
    spirit::rule<ScannerT, closures::Value<char>::context_t> arg;
    spirit::rule<ScannerT> functionBody;

    Name name;                    //Name grammar
    Statement statement;          //statement grammar

  };

};


template <>
Script::definition<Scanner>::definition(Script const & self);



} } } //namespace avs::parser::grammar

#endif //__AVS_PARSER_GRAMMAR_SCRIPT_H__
