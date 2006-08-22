// Avisynth v3.0 alpha.  Copyright 2003-2005 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_PARSER_VIRTUALMACHINE_H__
#define __AVS_PARSER_VIRTUALMACHINE_H__

//avisynth includes
#include "forward.h"
#include "avsvalue.h"
#include "../core/forward.h"                //for PEnvironment typedef

//boost include
#include <boost/shared_ptr.hpp>             //so PEnvironment is defined

//stl includes
#include <vector>
#include <stack>


namespace avs { namespace parser {



class VirtualMachine
{

  bool continue_;
  PEnvironment env_;


public:  //public members

  typedef std::vector<Operation>::const_iterator iterator;
  typedef std::stack<iterator> CallStack;
  typedef std::deque<AVSValue> Stack;

  iterator pc;                //program counter
  CallStack callStack;        //call stack
  
  Stack stack;


public:  //structors

  VirtualMachine(PEnvironment const& env)
    : env_( env ) { }

  //generated copy constructor and destructor are fine


public:  //interface

  void Run(iterator pc_);

  void Stop() { continue_ = false; }


public:

  PEnvironment const& GetEnvironment() const { return env_; }

};



} } //namespace avs::parser

#endif //__AVS_PARSER_VIRTUALMACHINE_H__
