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

//avisynth includes
#include "types.h"
#include "statement.h"


namespace avs { namespace parser { namespace grammar {



namespace closure {


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




} //namespace closure


struct Script : public spirit::grammar<Script, closure::Script::context_t>
{

  template <typename ScannerT>
  struct definition
  {

    definition(Script const& self)
    {

      using namespace lazy;
      using namespace phoenix;

      top
          =  *(   statement( CodeCouple(), self.localCtxt, self.globalCtxt, 'c' )
                  [
                    self.value += arg1       //accumulate code
                  ]
              |   spirit::str_p("function")
              >>  function
              |   spirit::eol_p
              )
          ;

      function
          =   Types::instance
              [
                first(function.ident) = arg1,
                second(function.localCtxt) = val(0)
              ]
          >>  name
              [
                second(function.ident) = construct_<std::string>(arg1, arg2)
              ]
          >>  '('
          >>  !(  arg   %   ','   )
          >>  spirit::ch_p(')')
              [
                bind(&function::Table::DeclareScriptFunction)( second(self.globalCtxt), function.ident ),
                function.termRecursive = val(false)       //init recursive flag
              ]
          >>  functionBody
          >>  spirit::eol_p
              [
                bind(&function::Table::DefineScriptFunction)
                    ( second(self.globalCtxt), function.ident, function.code, function.termRecursive )                    
              ]
          ;

      arg
        =   Types::instance
              [
                arg.value = arg1
              ]
          >>  (   name - spirit::lazy_p( first(function.localCtxt) )  )
              [
                bind(&VarTable::DefineVar)(first(function.localCtxt), construct_<std::string>(arg1, arg2), arg.value),
                third(function.ident) += arg.value,
                ++second(function.localCtxt)
              ]
          ;

      functionBody
          =   *   spirit::eol_p
          >>  '{'
          >> *(   statement
                      ( CodeCouple()
                      , ref(function.localCtxt)
                      , self.globalCtxt
                      , first(function.ident)
                      , construct_<value::TRecurseInfo>( ref(third(function.ident)), ref(function.termRecursive) ) 
                      ) 
                  [
                    function.code += arg1
                  ]
              |   spirit::eol_p
              )
          >>  '}'
          ;

    }


    spirit::rule<ScannerT> const & start() const { return top; }

  private:

    spirit::rule<ScannerT> top;
    spirit::rule<ScannerT, closure::Function::context_t> function;
    spirit::rule<ScannerT, closure::Value<char>::context_t> arg;
    spirit::rule<ScannerT> functionBody;

    Name name;                    //Name grammar
    Statement statement;          //statement grammar

  };

};



} } } //namespace avs::parser::grammar

#endif //__AVS_PARSER_GRAMMAR_SCRIPT_H__
