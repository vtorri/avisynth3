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


#ifndef __AVS_PARSER_FUNCTOR_CALLER_H__
#define __AVS_PARSER_FUNCTOR_CALLER_H__

//avisynth include
#include "extractor.h"
#include "../vmstate.h"

//boost includes
#include <boost/variant/get.hpp>
#include <boost/type_traits/function_traits.hpp>


namespace avs { namespace parser { namespace functor {


//implementation namespace
namespace detail {


template <typename Function, int n>
struct arg { };

template <typename Function>
struct arg<Function, 1>
{
  typedef typename boost::function_traits<Function>::arg1_type type;
};

template <typename Function>
struct arg<Function, 2>
{
  typedef typename boost::function_traits<Function>::arg2_type type;
};

template <typename Function>
struct arg<Function, 3>
{
  typedef typename boost::function_traits<Function>::arg3_type type;
};



template <typename Function, int n>
struct extract : public extractor<typename arg<Function, n>::type> { };





template <typename Function, int arity = boost::function_traits<Function>::arity>
struct caller_impl { };


template <typename Function>
struct caller_impl<Function, 0>
{

  enum { consume = 0 };

  typename boost::function_traits<Function>::result_type
  operator()(VMState& state, Function * function)
  {
    return function();
  }

};


template <typename Function>
struct caller_impl<Function, 1>
{

  enum { consume = extract<Function, 1>::consume };

  typename boost::function_traits<Function>::result_type
  operator()(VMState& state, Function * function)
  {
    return function( extract<Function, 1>()(state, 0) );
  }

};


template <typename Function>
struct caller_impl<Function, 2>
{

  enum { pos2 = 0, pos1 = extract<Function, 2>::consume, consume = pos1 + extract<Function, 1>::consume };

  typename boost::function_traits<Function>::result_type
  operator()(VMState& state, Function * function)
  {
    return function( extract<Function, 1>()(state, pos1), extract<Function, 2>()(state, pos2) );
  }

};


} //namespace detail


template <typename Function>
struct caller
{

  Function * function_;
  
  caller(Function * function)
    : function_( function ) { }

  enum { consume = detail::caller_impl<Function>::consume };

  typename boost::function_traits<Function>::result_type
  operator()(VMState& state) const
  {
    return detail::caller_impl<Function>()(state, function_);
  }

};


} } } //namespace avs::parser::functor

#endif //__AVS_PARSER_FUNCTOR_CALLER_H__
