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


#ifndef __AVS_PARSER_ADAPTOR_ADAPTOR_H__
#define __AVS_PARSER_ADAPTOR_ADAPTOR_H__

//avisynth includes
#include "../functor/popper.h"
#include "../functor/pusher.h"
#include "../functor/literal.h"


namespace avs { namespace parser { namespace adaptor {



////////////////////////////////////////////////////////////////////////////////////
//  adaptor<Caller, Result>
//
//  calls passed Caller, clean used args from the stack and push result on stack
//
template <typename Caller, typename Result = typename Caller::ResultType>
struct adaptor
{

  Caller caller_;

  enum { consume = Caller::consume };

  adaptor(Caller const& caller)
    : caller_( caller ) { }

  void operator()(VMState& state) const
  {
    functor::pusher<functor::literal<Result> > push( caller_(state) );
    functor::popper<consume>()(state);
    push(state);
  }

};


////////////////////////////////////////////////////////////////////////////////////
//  adaptor<Caller, void>
//
//  specialisation for the case where the Caller returns void
//
template <typename Caller>
struct adaptor<Caller, void>
{

  Caller caller_;

  enum { consume = Caller::consume };

  adaptor(Caller const& caller)
    : caller_( caller ) { }

  void operator()(VMState& state) const
  {
    caller_(state);
    functor::popper<consume>()(state);
  }

};





} } } //namespace avs::parser::adaptor

#endif //__AVS_PARSER_ADAPTOR_ADAPTOR_H__
