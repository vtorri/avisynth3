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


//avisynth includes
#include "action.h"
#include "../functor/var.h"
#include "../functor/popper.h"
#include "../functor/swapper.h"
#include "../functor/popassigner.h"


namespace avs { namespace parser { namespace action {


using namespace functor;


void Action::CreateVarStatement(CodeCouple& code, LocalContext& localCtxt, ExpressionValue const& expr, VarTable& table, std::string const& name)
{

  CleanOldLast(code, localCtxt, expr);                //get rid of old last

  int index = table.DefineVar(name, expr.template get<1>());      //defines the var in the given VarTable

  if ( &table != &localCtxt.template get<0>() )       //if a global var (table is not the local var Table)
  {
    code += popassigner<GlobalVar>( index );          //assign global var and pop value
    --localCtxt.template get<1>();                    //update stack size
  }

}


void Action::ExprStatement(CodeCouple& code, LocalContext& localCtxt, ExpressionValue const& expr)
{

  CleanOldLast(code, localCtxt, expr);                         //get rid of old last

  if ( expr.get<1>() == 'c' && ! expr.template get<2>() )      //if a non-assignment clip expr
    localCtxt.template get<2>() = localCtxt.template get<1>(); //define a new last
  else 
    if ( expr.template get<1>() != 'v' )                       //if expr actually stacked something
    {
      code += functor::popper<1>();                            //get rid of it
      --localCtxt.template get<1>();                           //update stack size
    }
}


void Action::CleanOldLast(CodeCouple& code, LocalContext& localCtxt, ExpressionValue const& expr)
{
  code += expr.template get<0>();          //accumulate expr generation code

  if ( !! localCtxt.template get<2>() )    //if there is an old last
  {
    localCtxt.template get<2>() = boost::detail::none_t();  //no last now

    if ( expr.template get<1>() != 'v' )   //if expr actually stacked something
      code += functor::Swapper();          //make old last top of stack
    code += functor::popper<1>();          //pop it

    --localCtxt.template get<1>();         //update stack size
  }
}


} } } //namespace avs::parser::action
