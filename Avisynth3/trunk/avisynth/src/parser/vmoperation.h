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


#ifndef __AVS_PARSER_VMOPERATION_H__
#define __AVS_PARSER_VMOPERATION_H__

//boost include
#include <boost/shared_ptr.hpp>


namespace avs { namespace parser {


class VMState;


//typedef boost::function<void (Stack&)> StackOperation


class VMOperation
{

  struct CallBack
  {
    virtual void operator()(VMState& state) const = 0;
  };

  template <class Functor>
  struct functor_callback : CallBack
  {
    Functor functor_;

    functor_callback(Functor const& functor)
      : functor_( functor ) { }

    virtual void operator()(VMState& state) const { functor_(state); }
  };


  boost::shared_ptr<CallBack const> cb_;


public:  //structors
  
  VMOperation() { }

  VMOperation(VMOperation const& other)
    : cb_( other.cb_ ) { }

  template <class Functor>
  VMOperation(Functor const& functor)
    : cb_( new functor_callback<Functor>(functor) ) { }


public:  //function-like interface

  void operator()(VMState& state) const { (*cb_)(state); }

  bool empty() const { return ! cb_; }

};


typedef std::pair<VMOperation, char> TypedOp;


} } //namespace avs::parser

#endif //__AVS_PARSER_VMOPERATION_H__
