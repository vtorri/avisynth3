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


#ifndef __AVS_PARSER_VMSTATE_H__
#define __AVS_PARSER_VMSTATE_H__

//avisynth includes
#include "avsvalue.h"
#include "../core/forward.h"  //for PEnvironment

//stl include
#include <vector>


namespace avs { namespace parser {


/////////////////////////////////////////////////////////////////////////////////////
//  VMState
//
//  virtual machine state
//
class VMState
{

  PEnvironment env_;
  std::vector<AVSValue> stack_;


public:  //structors

  VMState(PEnvironment const& env)
    : env_( env ) { }

  //generated destructor is fine


public:  //stack behavior

  void push(AVSValue const& value) { stack_.puh_back(value); }
  void pop() { stack_.pop_back(); }

  AVSValue& top() { return stack_.back(); }
  AVSValue& peek(int index) { return stack_[stack_.size() - 1 - index]; }

  AVSValue& operator[int index] { return stack_[index]; }


public:

  PEnvironment const& GetEnvironment() const { return env_; }

};



} } //namespace avs::parser

#endif //__AVS_PARSER_VMSTATE_H__
