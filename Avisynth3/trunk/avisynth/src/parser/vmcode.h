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

//avisynth include
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
class VMCode
{

  typedef std::vector<VMOperation> OperationVector;

  OperationVector code_;


public:  //structors

  VMCode() { }

  VMCode(VMOperation const& op) 
  {
    assert( ! op.empty() );
    code_.push_back(op);
  }

  //generated copy constructor and destructor are fine


public:  

  VMCode& operator+=(VMOperation const& op) 
  { 
    assert( ! op.empty() );
    code_.push_back(op); 
    return *this; 
  }

  VMCode& operator+=(VMCode const& other)
  {
    code_.insert(code_.end(), other.code_.begin(), other.code_.end());
    return *this;
  }


  void operator()(VMState& state) const
  {
    for( OperationVector::const_iterator it = code_.begin(); it != code_.end(); ++it )
      (*it)(state);
  }

  int size() const { return code_.size(); }

};


typedef boost::tuples::tuple<VMCode, char> TypedCode;


} } //namespace avs::parser

#endif //__AVS_PARSER_VMCODE_H__
