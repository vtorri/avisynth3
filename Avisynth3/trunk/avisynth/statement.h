// Avisynth v3.0 alpha.  Copyright 2003 Ben Rudiak-Gould et al.
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



#ifndef __STATEMENT_H__
#define __STATEMENT_H__


#include "expression.h"  //which includes vartable.h



class Statement {

public:
  Statement() { }
  virtual ~Statement() { }

  virtual void Execute(VarTable& table, ScriptEnvironment& env) = 0;

};

typedef boost::scoped_ptr<Statement> PStatement;


template <class T> class scoped_ptr_vector : public std::vector<T *> {

public:
  scoped_ptr_vector() : std::vector<T *>() { }
  scoped_ptr_vector(const scoped_ptr_vector<T>& other) : std::vector<T *>(other) { }

  ~scoped_ptr_vector() { for(iterator it = begin(); it != end(); ++it) delete *it; }

};


typedef scoped_ptr_vector<Statement> StatementVector;

class StatementBlock : public Statement {
  
  StatementVector statements;

public:
  StatementBlock(const StatementVector& _statements) : statements(_statements) { }

  virtual void Execute(VarTable& table, ScriptEnvironment& env)
  {
    for(StatementVector::iterator it = statements.begin(); it != statements.end(); ++it)
      (*it)->Execute(table, env);
  }
};


class StatementIfThen : public Statement {

protected:
  boost::scoped_ptr<Predicate> condition;
  PStatement then;

public:
  StatementIfThen(Predicate * _condition, Statement * _then) : condition(_condition), then(_then) { }

  virtual void Execute(VarTable& table, ScriptEnvironment& env)
  {
    DepthLock lock(table);
    if ( condition->TypeEval(table, env) )
      then->Execute(table, env);
  }
};

class StatementIfThenElse : public StatementIfThen {

  PStatement els;  //else is a reserved keyword

public:
  StatementIfThenElse(Predicate * condition, Statement * then, Statement * _els) : StatementIfThen(condition, then), els(_els) { }

  virtual void Execute(VarTable& table, ScriptEnvironment& env)
  {
    DepthLock lock(table);
    if ( condition->TypedEval(table, env) )
      then->Execute(table, env);
    else els->Execute(table, env);
  }

};

class StatementWhile : public Statement {

  boost::scoped_ptr<Predicate> whil;  //while
  PStatement d;                       //do
  
public:
  StatementWhile(Predicate * _whil, Statement * _d) : whil(_whil), d(_d) { }

  virtual void Execute(VarTable& table, ScriptEnvironment& env)
  {
    DepthLock lock(table);
    while( whil->TypedEval(table, env) )
      d->Execute(table, env);
  }
};

class StatementFor : public Statement {

  boost::scoped_ptr<Predicate> condition;
  PStatement init, next, loop;

public:
  StatementFor(Statement * _init, Predicate * _condition, Statement * _next, Statement * _loop)
    : init(_init), condition(_condition), next(_next), loop(_loop) { }

  virtual void Execute(VarTable& table, ScriptEnvironment& env)
  {
    DepthLock lock(table);
    for( init->Execute(table, env); condition->TypedEval(table, env); next->Execute(table, env) )
      loop->Execute(table, env);
  }
};


#endif //#ifndef __STATEMENT_H__