// Avisynth v3.0 alpha.  Copyright 2003-2006 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_PARSER_FUNCTOR_LITERAL_H__
#define __AVS_PARSER_FUNCTOR_LITERAL_H__

//avisynth include
#include "../virtualmachine.h"

//boost include
#include <boost/ref.hpp>


namespace avs { namespace parser { namespace functor {



/////////////////////////////////////////////////////////////////////////////////
//  literal<Value>
//
//  holds a value, used as template argument of the pusher functor
//
template <typename Value>
struct literal
{

  Value value_;

  literal(Value const& value)
    : value_( value ) { }

  literal(boost::reference_wrapper<Value> ref)
    : value_( ref.get() ) { }

  Value const& operator()(VirtualMachine& /*vm*/) const { return value_; }

};


} } } //namespace avs::parser::functor

#endif //__AVS_PARSER_FUNCTOR_LITERAL_H__
