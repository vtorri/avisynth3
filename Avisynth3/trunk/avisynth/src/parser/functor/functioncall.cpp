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


//avisynth includes
#include "../optype.h"
#include "../vmstate.h"
#include "functioncall.h"

//stl include
#include "assert.h"


namespace avs { namespace parser { namespace functor {



void FunctionCall::operator()(VMState& state) const
{
  int size = state.size() - arity_;

  OpType type = body_(state);
  assert( type == RETURN );

  AVSValue result = state.top();
  state.restore(size);
  state.push(result);
}


void VoidFunctionCall::operator()(VMState& state) const
{
  int size = state.size() - arity_;

  OpType type = body_(state);
  assert( type == RETURN );

  state.restore(size);
}


} } } //namespace avs::parser::functor
