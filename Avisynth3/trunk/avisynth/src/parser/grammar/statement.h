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


#ifndef __AVS_PARSER_GRAMMAR_STATEMENT_H__
#define __AVS_PARSER_GRAMMAR_STATEMENT_H__

//avisynth includes
#include "name.h"
#include "expression.h"
#include "../lazy/add_symbol.h"


namespace avs { namespace parser { namespace grammar {



class Statement : public spirit::grammar<Statement, closure::Value<VMCode>::context_t>
{

  int& stackSize;
  VarTable& varTable;


  struct popper
  {

    void operator()(Stack& stack) const { stack.pop(); }

  };


public:  //structors

  Statement(int& _stackSize, VarTable& _varTable)
    : stackSize( _stackSize )
    , varTable( _varTable ) { }


public:  //definition nested class

  template <typename ScannerT>
  struct definition
  {

    definition(Statement const& self)
      : expression( self.varTable )
    {

      using namespace lazy;
      using namespace phoenix;

      statement
          =   expression
              [
                self.value = first(arg1),
                self.value += construct_<popper>()
              ]
          |   createAssignVar
          ;

      createAssignVar
          =   name
              [
                createAssignVar.value = construct_<std::string>(arg1, arg2)
              ]
          >>  '='
          >>  expression
              [
                self.value = first(arg1),
                add_symbol(  var(self.varTable), createAssignVar.value, 
                    construct_<TypedIndex>( var(self.stackSize), second(arg1) )  ),                
                var(self.stackSize) += val(1)      
              ]
          ;

    }

    spirit::rule<ScannerT> const & start() const { return statement; }


  private:

    spirit::rule<ScannerT> statement;
    spirit::rule<ScannerT, closure::Value<std::string>::context_t> createAssignVar;

    Name name;
    Expression expression;

  };

};



} } } //namespace avs::parser::grammar

#endif //__AVS_PARSER_GRAMMAR_STATEMENT_H__
