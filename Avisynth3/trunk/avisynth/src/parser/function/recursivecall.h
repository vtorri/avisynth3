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


#ifndef __AVS_PARSER_FUNCTION_RECURSIVECALL_H__
#define __AVS_PARSER_FUNCTION_RECURSIVECALL_H__

//avisynth includes
#include "../optype.h"
#include "../vmcode.h"
#include "../vmstate.h"
#include "../forward.h"


namespace avs { namespace parser { namespace functor { namespace function {



//////////////////////////////////////////////////////////////////////////////////
//  RecursiveCall<bool returnVoid>
//
//  functor to make a function using terminal recursivity
//
template <bool returnVoid> struct RecursiveCall
{

  int const arity_;
  StatementCode const body_;

  RecursiveCall(int arity, StatementCode const& body)
    : arity_( arity )
    , body_( body ) { }

  void operator()(VMState& state) const
  {
    int size = state.size() - arity_;

    while( true )
    {
      OpType type = body_(state);
      
      if ( type == RETURN )
      {
        state.restore(size, returnVoid ? 0 : 1);   //clean stack of temporaries, keeping result if there is one
        break;                                     //exit while
      }

      assert( type == RECURSE );                   //else it must be terminal recursivity

      state.restore(size, arity_);                 //clean and keep args for another pass through the code
    }
  }

};


} } } } //namespace avs::parser::functor::function

#endif //__AVS_PARSER_FUNCTION_RECURSIVECALL_H__