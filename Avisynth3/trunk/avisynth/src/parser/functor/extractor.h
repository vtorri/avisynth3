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


#ifndef __AVS_PARSER_FUNCTOR_EXTRACTOR_H__
#define __AVS_PARSER_FUNCTOR_EXTRACTOR_H__

//avisynth include
#include "../vmstate.h"
#include "../../core/dimension.h"

//boost includes
#include <boost/variant/get.hpp>


namespace avs { namespace parser { namespace functor {



////////////////////////////////////////////////////////////////////////////////
//  extractor<Value>
//
//  functor to extracts values from the virtual machine state
//
template <typename Value>
struct extractor
{

  enum { consume = 1 };  //number of stack slots used to produce the value

  Value operator()(VMState& state, int position) const
  {
    return boost::get<Value>( state.peek(position) );
  }

};


template <>
struct extractor<Dimension const&>
{

  enum { consume = 2 };

  Dimension operator()(VMState& state, int pos) const
  {
    return Dimension( boost::get<int>(state.peek(pos + 1)), boost::get<int>(state.peek(pos)) );
  }

};


template <>
struct extractor<PEnvironment const&>
{

  enum { consume = 0 };

  PEnvironment const& operator()(VMState& state, int pos) const
  {
    return state.GetEnvironment();
  }

};



} } } //namespace avs::parser::extractor

#endif //__AVS_PARSER_FUNCTOR_EXTRACTOR_H__
