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


#ifndef __AVS_PARSER_FUNCTOR_VAR_H__
#define __AVS_PARSER_FUNCTOR_VAR_H__

//avisynth include
#include "../vmstate.h"


namespace avs { namespace parser { namespace functor {



//////////////////////////////////////////////////////////////////////////////////
//  LocalVar
//
//  provides access to a local var (ie stack based)
//
struct LocalVar
{

  int const pos_;           //position from the top of the var on the stack

  LocalVar(int pos)
    : pos_( pos ) { }

  AVSValue& operator()(VMState& state) const { return state.peek(pos_); }

};


///////////////////////////////////////////////////////////////////////////////////
//  GlobalVar
//
//  provides access to a global var
//
struct GlobalVar
{

  int const index_;

  GlobalVar(int index)
    : index_( index ) { }

  AVSValue& operator()(VMState& state) const { return state.global(index_); }

};


} } } //namespace avs::parser::functor

#endif //__AVS_PARSER_FUNCTOR_VAR_H__
