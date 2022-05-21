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


//avisynth includes
#include "call.h"
#include "forwardcall.h"
#include "recursivecall.h"
#include "scriptfunction.h"


namespace avs { namespace parser { namespace function {


char ScriptFunction::GetReturnType() const { return id_.get<0>(); }
char const * ScriptFunction::GetName() const { return id_.get<1>().c_str(); }
char const * ScriptFunction::GetPrototype() const { return id_.get<2>().c_str(); }
linker::PPlugin ScriptFunction::GetMotherPlugin() const { return linker::PPlugin(); }


void ScriptFunction::AppendOperation(ElementalCode& appendTo) const
{
  if ( op_.empty() )   //if op_ is not defined, we code a forward call to self
    appendTo += functor::function::ForwardCall(shared_from_this());
  else appendTo += op_;       //else op_ itself
}


void ScriptFunction::Define(StatementCode const& code, bool termRecursive)
{
  int arity = id_.get<2>().size();

  if ( termRecursive )
    if ( GetReturnType() == 'v' )
      op_ = functor::function::RecursiveCall<true>(arity, code);
    else
      op_ = functor::function::RecursiveCall<false>(arity, code);

  else
    if ( GetReturnType() == 'v' )
      op_ = functor::function::Call<true>(arity, code);
    else
      op_ = functor::function::Call<false>(arity, code);

}


} } } //namespace avs::parser::function
