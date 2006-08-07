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


#ifndef __AVS_PARSER_ADAPTOR_EXTRACTOR_H__
#define __AVS_PARSER_ADAPTOR_EXTRACTOR_H__

//avisynth includes
#include "../vmstate.h"
#include "../../core/colorspace.h"
#include "../../core/colorspace/get.h"
#include "../../core/geometry/dimension.h"

//boost includes
#include <boost/variant/get.hpp>


namespace avs { namespace parser { namespace adaptor {



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


////////////////////////////////////////////////////////////////////////////////
//  extractor<Value const&>
//
//  specialisation for extracting by ref to const
//
template <typename Value>
struct extractor<Value const&>
{

  enum { consume = 1 };

  Value const& operator()(VMState& state, int position) const
  {
    return boost::get<Value>( state.peek(position) );
  }

};


////////////////////////////////////////////////////////////////////////////////
//  extractor<Dimension const&>
//
//  Dimension extractor
//
template <>
struct extractor<Dimension const&>
{

  enum { consume = 2 };

  Dimension operator()(VMState& state, int pos) const
  {
    return Dimension( boost::get<int>(state.peek(pos + 1)), boost::get<int>(state.peek(pos)) );
  }

};


////////////////////////////////////////////////////////////////////////////////
//  extractor<PEnvironment const&>
//
//  PEnvironment extractor
//
template <>
struct extractor<PEnvironment const&>
{

  enum { consume = 0 };

  PEnvironment const& operator()(VMState& state, int /*pos*/) const
  {
    return state.GetEnvironment();
  }

};


////////////////////////////////////////////////////////////////////////////////
//  extractor<PColorSpace const&>
//
//  ColorSpace extractor, takes a string and converts it into a ColorSpace
//
template <>
struct extractor<PColorSpace const&>
{

  enum { consume = 1 };

  PColorSpace operator()(VMState& state, int pos) const
  {
    return colorspace::Get::FromName( boost::get<std::string>(state.peek(pos)) );
  }

};


} } } //namespace avs::parser::adaptor

#endif //__AVS_PARSER_ADAPTOR_EXTRACTOR_H__
