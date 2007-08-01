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


#ifndef __AVS_PARSER_ADAPTOR_CALLER_H__
#define __AVS_PARSER_ADAPTOR_CALLER_H__

//avisynth includes
#include "arg.h"
#include "extractor.h"
#include "../vmstate.h"

//boost includes
#include <boost/variant/get.hpp>


namespace avs { namespace parser { namespace adaptor { namespace detail {



/////////////////////////////////////////////////////////////////////////
//  extract<Function, int n>
//
//  combines the extractor and arg templates
//  in a functor extracting values of function arg n type from the stack
//
template <typename Function, int n>
struct extract : public extractor<typename arg<Function, n>::type> { };




////////////////////////////////////////////////////////////////////////////////////
//  caller_<Function, int arity> and its specialisations
//
//  handles the variable number of args for caller
//
template <typename Function, int arity = traits<Function>::arity>
struct caller_ { };


template <typename Function>
struct caller_<Function, 0>
{

  typedef typename traits<Function>::ResultType ResultType;

  enum { consume = 0 };

  template <typename CallBack>
  ResultType operator()(VMState& /*state*/, CallBack const& callBack) const
  {
    return callBack();
  }

};


template <typename Function>
struct caller_<Function, 1>
{

  typedef typename traits<Function>::ResultType ResultType;

  enum { consume = extract<Function, 1>::consume };

  template <typename CallBack>
  ResultType operator()(VMState& state, CallBack const& callBack) const
  {
    return callBack( extract<Function, 1>()(state, 1) );
  }

};


template <typename Function>
struct caller_<Function, 2>
{

  typedef typename traits<Function>::ResultType ResultType;

  enum { pos2 = 1
       , pos1 = pos2 + extract<Function, 2>::consume
       , consume = pos1 + extract<Function, 1>::consume - 1 
  };

  template <typename CallBack>
  ResultType operator()(VMState& state, CallBack const& callBack) const
  {
    return callBack
        ( extract<Function, 1>()(state, pos1)
        , extract<Function, 2>()(state, pos2) 
        );
  }

};


template <typename Function>
struct caller_<Function, 3>
{

  typedef typename traits<Function>::ResultType ResultType;

  enum { pos3 = 1
       , pos2 = pos3 + extract<Function, 3>::consume
       , pos1 = pos2 + extract<Function, 2>::consume
       , consume = pos1 + extract<Function, 1>::consume - 1 
  };

  template <typename CallBack>
  ResultType operator()(VMState& state, CallBack const& callBack) const
  {
    return callBack
        ( extract<Function, 1>()(state, pos1)
        , extract<Function, 2>()(state, pos2) 
        , extract<Function, 3>()(state, pos3)
        );
  }

};


template <typename Function>
struct caller_<Function, 4>
{

  typedef typename traits<Function>::ResultType ResultType;

  enum { pos4 = 1
       , pos3 = pos4 + extract<Function, 4>::consume
       , pos2 = pos3 + extract<Function, 3>::consume
       , pos1 = pos2 + extract<Function, 2>::consume
       , consume = pos1 + extract<Function, 1>::consume - 1 
  };

  template <typename CallBack>
  ResultType operator()(VMState& state, CallBack const& callBack) const
  {
    return callBack
        ( extract<Function, 1>()(state, pos1)
        , extract<Function, 2>()(state, pos2) 
        , extract<Function, 3>()(state, pos3)
        , extract<Function, 4>()(state, pos4)
        );
  }

};


template <typename Function>
struct caller_<Function, 5>
{

  typedef typename traits<Function>::ResultType ResultType;

  enum { pos5 = 1
       , pos4 = pos5 + extract<Function, 5>::consume
       , pos3 = pos4 + extract<Function, 4>::consume
       , pos2 = pos3 + extract<Function, 3>::consume
       , pos1 = pos2 + extract<Function, 2>::consume
       , consume = pos1 + extract<Function, 1>::consume - 1 
  };

  template <typename CallBack>
  ResultType operator()(VMState& state, CallBack const& callBack) const
  {
    return callBack
        ( extract<Function, 1>()(state, pos1)
        , extract<Function, 2>()(state, pos2) 
        , extract<Function, 3>()(state, pos3)
        , extract<Function, 4>()(state, pos4)
        , extract<Function, 5>()(state, pos5)
        );
  }

};


template <typename Function>
struct caller_<Function, 6>
{

  typedef typename traits<Function>::ResultType ResultType;

  enum { pos6 = 1
       , pos5 = pos6 + extract<Function, 6>::consume
       , pos4 = pos5 + extract<Function, 5>::consume
       , pos3 = pos4 + extract<Function, 4>::consume
       , pos2 = pos3 + extract<Function, 3>::consume
       , pos1 = pos2 + extract<Function, 2>::consume
       , consume = pos1 + extract<Function, 1>::consume - 1 
  };

  template <typename CallBack>
  ResultType operator()(VMState& state, CallBack const& callBack) const
  {
    return callBack
        ( extract<Function, 1>()(state, pos1)
        , extract<Function, 2>()(state, pos2) 
        , extract<Function, 3>()(state, pos3)
        , extract<Function, 4>()(state, pos4)
        , extract<Function, 5>()(state, pos5)
        , extract<Function, 6>()(state, pos6)
        );
  }

};


template <typename Function>
struct caller_<Function, 7>
{

  typedef typename traits<Function>::ResultType ResultType;

  enum { pos7 = 1
       , pos6 = pos7 + extract<Function, 7>::consume
       , pos5 = pos6 + extract<Function, 6>::consume
       , pos4 = pos5 + extract<Function, 5>::consume
       , pos3 = pos4 + extract<Function, 4>::consume
       , pos2 = pos3 + extract<Function, 3>::consume
       , pos1 = pos2 + extract<Function, 2>::consume
       , consume = pos1 + extract<Function, 1>::consume - 1 
  };

  template <typename CallBack>
  ResultType operator()(VMState& state, CallBack const& callBack) const
  {
    return callBack
        ( extract<Function, 1>()(state, pos1)
        , extract<Function, 2>()(state, pos2) 
        , extract<Function, 3>()(state, pos3)
        , extract<Function, 4>()(state, pos4)
        , extract<Function, 5>()(state, pos5)
        , extract<Function, 6>()(state, pos6)
        , extract<Function, 7>()(state, pos7)
        );
  }

};


template <typename Function>
struct caller_<Function, 8>
{

  typedef typename traits<Function>::ResultType ResultType;

  enum { pos8 = 1
       , pos7 = pos8 + extract<Function, 8>::consume
       , pos6 = pos7 + extract<Function, 7>::consume
       , pos5 = pos6 + extract<Function, 6>::consume
       , pos4 = pos5 + extract<Function, 5>::consume
       , pos3 = pos4 + extract<Function, 4>::consume
       , pos2 = pos3 + extract<Function, 3>::consume
       , pos1 = pos2 + extract<Function, 2>::consume
       , consume = pos1 + extract<Function, 1>::consume - 1 
  };

  template <typename CallBack>
  ResultType operator()(VMState& state, CallBack const& callBack) const
  {
    return callBack
        ( extract<Function, 1>()(state, pos1)
        , extract<Function, 2>()(state, pos2) 
        , extract<Function, 3>()(state, pos3)
        , extract<Function, 4>()(state, pos4)
        , extract<Function, 5>()(state, pos5)
        , extract<Function, 6>()(state, pos6)
        , extract<Function, 7>()(state, pos7)
        , extract<Function, 8>()(state, pos8)
        );
  }

};


template <typename Function>
struct caller_<Function, 9>
{

  typedef typename traits<Function>::ResultType ResultType;

  enum { pos9 = 1
       , pos8 = pos9 + extract<Function, 9>::consume
       , pos7 = pos8 + extract<Function, 8>::consume
       , pos6 = pos7 + extract<Function, 7>::consume
       , pos5 = pos6 + extract<Function, 6>::consume
       , pos4 = pos5 + extract<Function, 5>::consume
       , pos3 = pos4 + extract<Function, 4>::consume
       , pos2 = pos3 + extract<Function, 3>::consume
       , pos1 = pos2 + extract<Function, 2>::consume
       , consume = pos1 + extract<Function, 1>::consume - 1 
  };

  template <typename CallBack>
  ResultType operator()(VMState& state, CallBack const& callBack) const
  {
    return callBack
        ( extract<Function, 1>()(state, pos1)
        , extract<Function, 2>()(state, pos2) 
        , extract<Function, 3>()(state, pos3)
        , extract<Function, 4>()(state, pos4)
        , extract<Function, 5>()(state, pos5)
        , extract<Function, 6>()(state, pos6)
        , extract<Function, 7>()(state, pos7)
        , extract<Function, 8>()(state, pos8)
        , extract<Function, 9>()(state, pos9)
        );
  }

};

} //namespace detail



//////////////////////////////////////////////////////////////////////////////
//  caller<CallBack, Function>
//
//  calls CallBack with args corresponding to the prototype of Function
//
template <typename CallBack, typename Function>
struct caller : detail::caller_<Function>
{

  CallBack const callBack_;

  caller(CallBack const& callBack)
    : callBack_( callBack ) { }

  typename detail::caller_<Function>::ResultType
  operator()(VMState& state) const
  {
    return detail::caller_<Function>::operator()(state, callBack_);
  }

};
  


} } } //namespace avs::parser::adaptor

#endif //__AVS_PARSER_ADAPTOR_CALLER_H__
