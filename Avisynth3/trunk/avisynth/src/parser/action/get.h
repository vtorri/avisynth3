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


#ifndef __AVS_PARSER_ACTION_GET_H__
#define __AVS_PARSER_ACTION_GET_H__

//avisynth includes
#include "../forward.h"
#include "../vmoperation.h"


namespace avs { namespace parser { namespace action {



//////////////////////////////////////////////////////////////////////////////////
//  Get
//
//  provides envelopping for some static get type functions
//
class Get
{

  static ElementalOperation const equal_op[5];
  static ElementalOperation const not_equal_op[5];


public:  //Get methods

  //get appropriate version of the operator[]
  static ElementalOperation SubscriptOperation(char type);
  static ElementalOperation OneArgSubscriptOperation(char type);
  static ElementalOperation NegEndSubscriptOperation(char type);
  
  //get operation for operator== or operator!=, or throw if types don't allow comparison 
  static ElementalOperation const& EqualityOperation(char& leftType, char rightType, bool isEqual);

  //get approprivate binary operation given operands types and op symbol
  static ElementalOperation const& BinaryOperation(char& leftType, char rightType, char opSymbol);


private:  //implementation helper

  static int TypeToIndex(char type);

};


} } } //namespace avs::parser::action

#endif //__AVS_PARSER_ACTION_GET_H__
