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

#ifndef __AVS_PARSER_BINARYOP_TYPEMAPPED_H__
#define __AVS_PARSER_BINARYOP_TYPEMAPPED_H__

//avisynth include
#include "../vmoperation.h"

//stl include
#include <map>
#include <string>


namespace avs { namespace parser { namespace binaryop {



////////////////////////////////////////////////////////////////////////////
//  TypeMapped
//
//  associates corresponding stack operation to a binary operation
//  and deduces return type from types of the two args
//
class TypeMapped
{

  typedef std::map<std::pair<char, char>, char> TypeMap;
   
  TypeMap map_;
  VMOperation<void> op_;
  std::string opName_;


public:  //structors

  TypeMapped(std::string const& opName, VMOperation<void> const& op, std::string const types);

  //generated copy constructor and destructor are fine


public:  //interface

  //get return type given types of left and right args
  char get(char left, char right) const;

  VMOperation<void> const& op() const { return op_; }

};




} } } //namespace avs::parser::binaryop

#endif //__AVS_PARSER_BINARYOP_TYPEMAPPED_H__
