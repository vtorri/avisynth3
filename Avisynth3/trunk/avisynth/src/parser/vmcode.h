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


#ifndef __AVS_PARSER_VMCODE_H__
#define __AVS_PARSER_VMCODE_H__

//avisynth includes
#include "optype.h"
#include "vmoperation.h"

//stl include
#include <vector>
#include <assert.h>


namespace avs { namespace parser {



/////////////////////////////////////////////////////////////////////////////
//  VMCode
//
//  code for the virtual machine
//
template <typename Result> class VMCode
{

  typedef std::vector<VMOperation<Result> > OperationVector;

  OperationVector code_;


public:  //structors

  VMCode() { }

  VMCode(VMOperation<Result> const& op) 
  {
    assert( ! op.empty() );
    code_.push_back(op);
  }

  //generated copy constructor and destructor are fine


public:  //code accumulation methods

  VMCode<Result>& operator+=(VMOperation<Result> const& op) 
  { 
    assert( ! op.empty() );
    code_.push_back(op); 
    return *this; 
  }

  VMCode<Result>& operator+=(VMCode<Result> const& other)
  {
    code_.insert(code_.end(), other.code_.begin(), other.code_.end());
    return *this;
  }


public:  //stack functor method

  Result operator()(VMState& state) const;


public:  //misc

  int size() const { return code_.size(); }

};


template <>
void VMCode<void>::operator()(VMState& state) const;

template <>
OpType VMCode<OpType>::operator()(VMState& state) const;



} } //namespace avs::parser

#endif //__AVS_PARSER_VMCODE_H__
