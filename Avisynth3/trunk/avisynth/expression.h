// Avisynth v2.5.  Copyright 2002 Ben Rudiak-Gould et al.
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
//
// As a special exception, the copyright holders of Avisynth give you
// permission to link Avisynth with independent modules that communicate with
// Avisynth solely through the interfaces defined in avisynth.h, regardless of the license
// terms of these independent modules, and to copy and distribute the
// resulting combined work under terms of your choice, provided that
// every copy of the combined work is accompanied by a complete copy of
// the source code of Avisynth (the version of Avisynth used to produce the
// combined work), being distributed under the terms of the GNU General
// Public License plus this exception.  An independent module is a module
// which is not derived from or based on Avisynth, such as 3rd-party filters,
// import and export plugins, or graphical user interfaces.

#ifndef __EXPRESSSION_H__
#define __EXPRESSSION_H__

#include "vartable.h"
#include <boost/scoped_ptr.hpp>


class ScriptEnvironment;


class Expression {

public:
  Expression() { }
  virtual ~Expression() {}

  virtual AVSValue Evaluate(VarTable& table, ScriptEnvironment& env) = 0;


};

template <class Result> class TypedExpression : public Expression {

public:
  TypedExpression() { }

  //default implementation using the new TypedEval method
  virtual AVSValue Evaluate(VarTable& table, ScriptEnvironment& env) { return AVSValue(TypedEval(table, env)); }
  virtual Result TypedEval(VarTable& table, ScriptEnvironment& env) = 0;

};

typedef TypedExpression<bool> Predicate;


template <class Result> class ExpConstant : public TypedExpression<Result> {

  Result result;

public:
  ExpConstant(Result _result) : result(_result) { }

  virtual AVSValue Evaluate(VarTable& table, ScriptEnvironment& env) { return AVSValue(result); }
  virtual Result TypedEval(VarTable& table, ScriptEnvironment& env) { return result; }

};



template <class Result, class Arg> class BinaryExp : public TypedExpression<Result> {

protected:
  typedef TypedExpression<Arg>  ExpBase;
  typedef boost::scoped_ptr<ExpBase> ScopedExpBase;

  ScopedExpBase left, right;

public:
  BinaryExp(ExpBase * _left, ExpBase * _right) : left(_left), right(_right) { }


};

//basic binaries operations

template <class T> ExpPlus : public BinaryExp<T, T> {

public:
  ExpPlus(ExpBase * left, ExpBase * right) : BinaryExp<T, T>(left, right) { }

  virtual T TypedEval(VarTable& table, ScriptEnvironment& env) { return left->TypedEval(table, env) + right->TypedEval(table, env); }
};


template <class T> ExpMinus : public BinaryExp<T, T> {

public:
  ExpMinus(ExpBase * left, ExpBase * right) : BinaryExp<T, T>(left, right) { }

  virtual T TypedEval(VarTable& table, ScriptEnvironment& env) { return left->TypedEval(table, env) - right->TypedEval(table, env); }
};

template <class T> ExpMult : public BinaryExp<T, T> {

public:
  ExpMult(ExpBase * left, ExpBase * right) : BinaryExp<T, T>(left, right) { }

  virtual T TypedEval(VarTable& table, ScriptEnvironment& env) { return left->TypedEval(table, env) * right->TypedEval(table, env); }
};

template <class T> ExpDiv : public BinaryExp<T, T> {

public:
  ExpDiv(ExpBase * left, ExpBase * right) : BinaryExp<T, T>(left, right) { }

  virtual T TypedEval(VarTable& table, ScriptEnvironment& env) { return left->TypedEval(table, env) / right->TypedEval(table, env); }
};

template <class T> ExpModulo : public BinaryExp<T, T> {

public:
  ExpDiv(ExpBase * left, ExpBase * right) : BinaryExp<T, T>(left, right) { }

  virtual T TypedEval(VarTable& table, ScriptEnvironment& env) { return left->TypedEval(table, env) % right->TypedEval(table, env); }
};


//binaries predicates

template <class T> ExpEqual : public BinaryExp<bool, T> {

public:
  ExpEqual(ExpBase * left, ExpBase * right) : BinaryExp<bool, T>(left, right) { }

  virtual bool TypedEval(VarTable& table, ScriptEnvironment& env) { return left->TypedEval(table, env) == right->TypedEval(table, env); }  
};

template <class T> ExpLess : public BinaryExp<bool, T> {

public:
  ExpLess(ExpBase * left, ExpBase * right) : BinaryExp<bool, T>(left, right) { }

  virtual bool TypedEval(VarTable& table, ScriptEnvironment& env) { return left->TypedEval(table, env) < right->TypedEval(table, env); }  
};




/**** Object classes ****/



class ExpSequence : public Expression 
{
public:
  ExpSequence(const PExpression& _a, const PExpression& _b) : a(_a), b(_b) {}
  virtual AVSValue Evaluate(IScriptEnvironment* env);  
private:
  const PExpression a, b;
};


class ExpExceptionTranslator : public Expression 
{
public:
  ExpExceptionTranslator(const PExpression& _exp) : exp(_exp) {}
  AVSValue Evaluate(IScriptEnvironment* env);
  
private:
  const PExpression exp;
};


class ExpTryCatch : public ExpExceptionTranslator 
{
public:
  ExpTryCatch(const PExpression& _try_block, const char* _id, const PExpression& _catch_block)
    : ExpExceptionTranslator(_try_block), id(_id), catch_block(_catch_block) {}
  AVSValue Evaluate(IScriptEnvironment* env);  

private:
  const char* const id;
  const PExpression catch_block;
};

class ExpLine : public ExpExceptionTranslator 
{
public:
  ExpLine(const PExpression& _exp, const char* _filename, int _line)
    : ExpExceptionTranslator(_exp), filename(_filename), line(_line) {}
  AVSValue Evaluate(IScriptEnvironment* env);
  
private:
  const char* const filename;
  const int line;
};


class ExpConditional : public Expression 
{
public:
  ExpConditional(const PExpression& _If, const PExpression& _Then, const PExpression& _Else)
   : If(_If), Then(_Then), Else(_Else) {}
  virtual AVSValue Evaluate(IScriptEnvironment* env);
  
private:
  const PExpression If, Then, Else;
};





/**** Operator classes ****/

class ExpOr : public Expression 
{
public:
  ExpOr(const PExpression& _a, const PExpression& _b) : a(_a), b(_b) {}
  virtual AVSValue Evaluate(IScriptEnvironment* env);
  
private:
  const PExpression a, b;
};


class ExpAnd : public Expression 
{
public:
  ExpAnd(const PExpression& _a, const PExpression& _b) : a(_a), b(_b) {}
  virtual AVSValue Evaluate(IScriptEnvironment* env);
  
private:
  const PExpression a, b;
};


class ExpEqual : public Expression 
{
public:
  ExpEqual(const PExpression& _a, const PExpression& _b) : a(_a), b(_b) {}
  virtual AVSValue Evaluate(IScriptEnvironment* env);
  
private:
  const PExpression a, b;
};


class ExpLess : public Expression 
{
public:
  ExpLess(const PExpression& _a, const PExpression& _b) : a(_a), b(_b) {}
  virtual AVSValue Evaluate(IScriptEnvironment* env); 
  
private:
  const PExpression a, b;
};


class ExpPlus : public Expression 
{
public:
  ExpPlus(const PExpression& _a, const PExpression& _b) : a(_a), b(_b) {}
  virtual AVSValue Evaluate(IScriptEnvironment* env);

private:
  const PExpression a, b;
};


class ExpDoublePlus : public Expression 
{
public:
  ExpDoublePlus(const PExpression& _a, const PExpression& _b) : a(_a), b(_b) {}
  virtual AVSValue Evaluate(IScriptEnvironment* env);
  
private:
  const PExpression a, b;
};








class ExpNegate : public Expression 
{
public:
  ExpNegate(const PExpression& _e) : e(_e) {}
virtual AVSValue Evaluate(IScriptEnvironment* env);

private:
  const PExpression e;
};


class ExpNot : public Expression 
{
public:
  ExpNot(const PExpression& _e) : e(_e) {}
  virtual AVSValue Evaluate(IScriptEnvironment* env);

private:
  const PExpression e;
};


class ExpVariableReference : public Expression 
{
public:
  ExpVariableReference(const char* _name) : name(_name) {}
  virtual AVSValue Evaluate(IScriptEnvironment* env);
  
  virtual const char* GetLvalue() { return name; }

private:
  const char* const name;
};


class ExpAssignment : public Expression 
{
public:
  ExpAssignment(const char* _lhs, const PExpression& _rhs) : lhs(_lhs), rhs(_rhs) {}
  virtual AVSValue Evaluate(IScriptEnvironment* env);

private:
  const char* const lhs;
  PExpression rhs;
};


class ExpGlobalAssignment : public Expression 
{
public:
  ExpGlobalAssignment(const char* _lhs, const PExpression& _rhs) : lhs(_lhs), rhs(_rhs) {}
  virtual AVSValue Evaluate(IScriptEnvironment* env);
  
private:
  const char* const lhs;
  PExpression rhs;
};


class ExpFunctionCall : public Expression 
{
public:
  ExpFunctionCall( const char* _name, PExpression* _arg_exprs,
                   const char** _arg_expr_names, int _arg_expr_count, bool _oop_notation );  
  ~ExpFunctionCall(void);
  
  AVSValue Call(IScriptEnvironment* env);
  virtual AVSValue Evaluate(IScriptEnvironment* env);
  
private:
  const char* const name;
  PExpression* arg_exprs;
  const char** arg_expr_names;
  const int arg_expr_count;
  const bool oop_notation;
};



#endif  // __EXPRESSSION_H__