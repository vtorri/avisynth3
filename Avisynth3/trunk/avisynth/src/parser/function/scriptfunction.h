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


#ifndef __AVS_PARSER_FUNCTION_SCRIPTFUNCTION_H__
#define __AVS_PARSER_FUNCTION_SCRIPTFUNCTION_H__

//avisynth includes
#include "forward.h"
#include "../forward.h"
#include "../vmoperation.h"
#include "../../linker/function.h"

//boost include
#include <boost/enable_shared_from_this.hpp>  //for enable_shared_from_this


namespace avs { namespace parser { namespace function {



////////////////////////////////////////////////////////////////////////////
//  ScriptFunction
//
//
//
class ScriptFunction : public linker::Function
                     , public boost::enable_shared_from_this<ScriptFunction>
{

  FunctionId const id_;
  ElementalOperation op_;  //underlying operation


public:  //structors

  ScriptFunction(FunctionId const& id)
    : id_( id ) { }

  //generated destructor is fine


public:  //Function interface

  virtual char GetReturnType() const;
  virtual char const * GetName() const;
  virtual char const * GetPrototype() const;
  virtual linker::PPlugin GetMotherPlugin() const;

  virtual void AppendOperation(TypedCode& appendTo) const;


private:

  //defines self to execute the given code
  void Define(StatementCode const& code);

  friend class Table;                            //so it can call the above
  friend struct functor::function::ForwardCall;  //so it can access op_

};


} } } //namespace avs::parser

#endif //__AVS_PARSER_FUNCTION_SCRIPTFUNCTION_H__
