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
#include "if.h"
#include "../scopeenforcer.h"

//boost include
#include <boost/variant/get.hpp>


namespace avs { namespace parser { namespace functor {



OpType IfThen::operator ()(VMState& state) const
{
  OpType result = NORMAL;    
  bool cond = boost::get<bool>(state.top());
  state.pop();

  if ( cond )
  {
    ScopeEnforcer enforcer(state);

    result = then_(state);

    enforcer.SetDismiss( IsReturning(result) );
  }

  return result;
}


OpType IfThenElse::operator ()(VMState& state) const
{
  bool cond = boost::get<bool>(state.top());
  state.pop();

  ScopeEnforcer enforcer(state);

  OpType result = cond ? then_(state) : else_(state);

  enforcer.SetDismiss( IsReturning(result) );

  return result;
}



} } } //namespace avs::parser::functor
