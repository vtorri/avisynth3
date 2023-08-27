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


#ifndef __AVS_PARSER_VMOPERATION_H__
#define __AVS_PARSER_VMOPERATION_H__

//stl include
#include <memory>


namespace avs { namespace parser {


//declaration
class VMState;


//implementation namespace
namespace detail {


template <typename Result>
struct CallBack
{
  virtual ~CallBack() { }
  virtual Result operator()(VMState& state) const = 0;
};

template <typename Result, typename Functor>
struct functor_callback : CallBack<Result>
{

  Functor const functor_;

  functor_callback(Functor const& functor)
    : functor_( functor ) { }

  virtual Result operator()(VMState& state) const { return functor_(state); }

};

template <typename Functor>
struct functor_callback<void, Functor> : CallBack<void>
{

  Functor const functor_;

  functor_callback(Functor const& functor)
    : functor_( functor ) { }

  virtual void operator()(VMState& state) const { functor_(state); }

};


}  //namespace detail



///////////////////////////////////////////////////////////////////////////////////
//  VMOperation<Result>
//
//  operation of the virtual machine
//
template <typename Result> class VMOperation
{

  std::shared_ptr<detail::CallBack<Result> const> cb_;


public:

  typedef Result result_type;


public:  //structors

  VMOperation() { }

  VMOperation(VMOperation const& other)
    : cb_( other.cb_ ) { }

  template <class Functor>
  VMOperation(Functor const& functor)
    : cb_( new detail::functor_callback<Result, Functor>(functor) ) { }


public:  //assignment

  template <class Functor>
  VMOperation<Result>& operator=(Functor const& functor)
  {
    cb_.reset( new detail::functor_callback<Result, Functor>(functor) );
    return *this;
  }


public:  //function-like interface

  Result operator()(VMState& state) const { return (*cb_)(state); }

  bool empty() const { return ! cb_; }

};



} } //namespace avs::parser

#endif //__AVS_PARSER_VMOPERATION_H__
