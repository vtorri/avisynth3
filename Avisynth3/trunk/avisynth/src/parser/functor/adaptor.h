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


#ifndef __AVS_PARSER_FUNCTOR_ADAPTOR_H__
#define __AVS_PARSER_FUNCTOR_ADAPTOR_H__

//avisynth includes
#include "caller.h"
#include "popper.h"
#include "pusher.h"
#include "literal.h"
#include "../vmstate.h"


namespace avs { namespace parser { namespace functor {


//implementation namespace
namespace detail {


template <typename Function, typename Result = typename boost::function_traits<Function>::result_type>
struct adaptor_impl
{

  caller<Function> caller;

  enum { consume = caller<Function>::consume };

  adaptor_impl(Function * function)
    : caller( function ) { }

  OpType operator()(VMState& state) const
  {
    pusher<literal<Result> > push( caller(state) );
    popper<consume>()(state);
    return push(state);
  }

};


template <typename Function>
struct adaptor_impl<Function, void>
{

  caller<Function> caller;

  enum { consume = caller<Function>::consume };

  adaptor_impl(Function * function)
    : caller( function ) { }

  OpType operator()(VMState& stack) const
  {
    caller<Function>::operator()(state);
    return popper<consume>()(state);
  }

};


} //namespace detail



/////////////////////////////////////////////////////////////////////////////////////////
//  adaptor<Function>
//
//  adapts a C++ free function in a functor operating on the virtual machine,
//  taking its argument from the stack, performing arg cleanup and pushing
//  its result onto the stack
//
template <typename Function>
struct adaptor : public detail::adaptor_impl<Function>
{
  
  adaptor(Function * function)
    : detail::adaptor_impl<Function>( function ) { }

};


/////////////////////////////////////////////////////////////////////////////////////////
//  adapt(Function * function)
//
//  wraps the appropriate adaptor around a free function
//
template <typename Function>
adaptor<Function> adapt(Function * function)
{
  return adaptor<Function>(function);
}



} } } //namespace avs::parser::functor

#endif //__AVS_PARSER_FUNCTOR_ADAPTOR_H__
