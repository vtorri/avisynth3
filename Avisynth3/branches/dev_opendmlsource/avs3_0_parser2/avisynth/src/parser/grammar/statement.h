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


#ifndef __AVS_PARSER_GRAMMAR_STATEMENT_H__
#define __AVS_PARSER_GRAMMAR_STATEMENT_H__

//avisynth includes
#include "name.h"
#include "expression.h"
#include "../codecouple.h"



namespace avs { namespace parser { namespace grammar {



namespace closure {


/////////////////////////////////////////////////////////////////////////////////
//  closure::Statement
//
//  closure for the Statement grammar
//
struct Statement : spirit::closure
    < Statement
    , CodeCouple
    , boost::reference_wrapper<LocalContext>
    , boost::reference_wrapper<GlobalContext>
    , char
    , boost::optional<value::TRecurseInfo>
    >
{
  member1 value;
  member2 localCtxt;
  member3 globalCtxt;
  member4 returnTypeExpected;
  member5 termRecInfo;
};


/////////////////////////////////////////////////////////////////////////////////
//  closure::InnerStatement
//
//  closure for the statement rule within the Statement grammar
//
struct InnerStatement : spirit::closure
    < InnerStatement
    , CodeCouple
    , boost::reference_wrapper<LocalContext>
    >
{
  member1 value;
  member2 localCtxt;
};


struct CreateVar : spirit::closure
    < CreateVar
    , boost::reference_wrapper<VarTable>
    , std::string
    >
{
  member1 table;
  member2 name;
};


struct StatementBlock : spirit::closure
    < StatementBlock
    , CodeCouple
    , LocalContext
    >
{
  member1 value;
  member2 localCtxt;
};


} //namespace closure




struct Statement : public spirit::grammar<Statement, closure::Statement::context_t>
{

  template <typename ScannerT>
  struct definition
  {

    definition(Statement const& self);


    spirit::rule<ScannerT> const & start() const { return top; }


  private:

    spirit::rule<ScannerT> top;
    spirit::rule<ScannerT, closure::InnerStatement::context_t> statement;
    spirit::rule<ScannerT, closure::CreateVar::context_t> createVar;
    spirit::rule<ScannerT, closure::Value<StatementCode>::context_t> ifStatement;
    spirit::rule<ScannerT, closure::Value<ElementalCode>::context_t> whileStatement;
    spirit::rule<ScannerT, closure::Value<ElementalCode>::context_t> conditionBlock;
    spirit::rule<ScannerT, closure::StatementBlock::context_t> subContextBlock;
    spirit::rule<ScannerT> returnStatement;
    spirit::rule<ScannerT, closure::Value<int>::context_t> argList;

    Name name;
    Expression expression;

  };

};


template <>
Statement::definition<Scanner>::definition(Statement const & self);


} } } //namespace avs::parser::grammar

#endif //__AVS_PARSER_GRAMMAR_STATEMENT_H__
