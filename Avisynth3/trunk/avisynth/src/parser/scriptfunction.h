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


#ifndef __AVS_PARSER_SCRIPTFUNCTION_H__
#define __AVS_PARSER_SCRIPTFUNCTION_H__

//avisynth includes
#include "vmoperation.h"
#include "../linker/function.h"


namespace avs { namespace parser {



class ScriptFunction : public linker::Function
{

  char const returnType_;
  std::string const name_;
  std::string const prototype_;
  ElementalOperation const op_;


public:  //structors

  ScriptFunction(char returnType, std::string const& name, std::string const& prototype, ElementalOperation const& op)
    : returnType_( returnType )
    , name_( name )
    , prototype_( prototype )
    , op_( op ) { }

  //generated destructor is fine


public:  //Function interface

  virtual char GetReturnType() const { return returnType_; }  
  virtual char const * GetName() const { return name_.c_str(); }
  virtual char const * GetPrototype() const { return prototype_.c_str(); }
  virtual linker::PPlugin GetMotherPlugin() const { return linker::PPlugin(); }

  virtual parser::ElementalOperation const& GetOperation() const { return op_; }

};


} } //namespace avs::parser

#endif //__AVS_PARSER_SCRIPTFUNCTION_H__
