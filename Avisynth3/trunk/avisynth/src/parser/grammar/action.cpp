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


//avisynth includes
#include "action.h"
#include "../functor/var.h"
#include "../functor/popper.h"
#include "../functor/swapper.h"
#include "../functor/popassigner.h"


namespace avs { namespace parser { namespace grammar {


using namespace functor;


void Action::CreateVarStatement(CodeCouple& code, LocalContext& localCtxt, value::Expression const& expr, VarTable& table, std::string const& name)
{

  code += expr.get<0>();                              //accumulate expr generation code

  int index = table.DefineVar(name, expr.get<1>());   //defines the var in the given VarTable

  if ( &table != &localCtxt.get<0>() )                //if a global var (table is not the local var Table)
  {
    code += popassigner<GlobalVar>( index );          //assign global var and pop value
    --localCtxt.get<1>();                             //update stack size
  }

  if ( !! localCtxt.get<2>() )                        //if was a last before
  {
    localCtxt.get<2>() = boost::detail::none_t();     //no last now

    CleanOldLast( code, &table == &localCtxt.get<0>(), localCtxt.get<1>() );
  }

}


void Action::ExprStatement(CodeCouple& code, LocalContext& localCtxt, value::Expression const& expr)
{

  bool oldLast = !! localCtxt.get<2>();           //mark if an old last has to be taken care of
  bool stacking = true;                           //expect to stack something

  code += expr.get<0>();                          //accumulate expr generation code

  if ( expr.get<1>() == 'c' && ! expr.get<2>() )  //if a non-assignment clip expr
    localCtxt.get<2>() = localCtxt.get<0>().size();   //define a new last
  else 
  {
    localCtxt.get<2>() = boost::detail::none_t(); //no last now

    if ( expr.get<1>() != 'v' )                   //if expr actually stacked something
    {
      code += functor::popper<1>();               //get rid of it
      --localCtxt.get<1>();                       //update stack size
    }

    stacking = false;                             //report that finally nothing was stacked
  }

  if ( oldLast )
    CleanOldLast(code, stacking, localCtxt.get<1>());

}


void Action::CleanOldLast(CodeCouple& code, bool stacking, int& stackSize)
{
  if ( stacking )                     //if something was stacked
    code += functor::Swapper();       //make last top of stack

  code += functor::popper<1>();       //pop it
  --stackSize;                        //update stack size
}


} } } //namespace avs::parser::grammar
