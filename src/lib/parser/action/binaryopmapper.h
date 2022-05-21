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


#ifndef __AVS_PARSER_ACTION_BINARYOPMAPPER_H__
#define __AVS_PARSER_ACTION_BINARYOPMAPPER_H__

//avisynth includes
#include "../forward.h"
#include "../vmoperation.h"

//boost include
#include <boost/utility.hpp>    //for noncopyable

//stl include
#include <map>


namespace avs { namespace parser { namespace action {



class BinaryOpMapper : public boost::noncopyable
{

  typedef std::pair<ElementalOperation, char> TypedOp;
  typedef std::map<std::pair<char, char>, TypedOp> TypedOpMap;

  TypedOpMap plusMap_;         //maps + operations
  TypedOpMap minusMap_;        //maps - ops
  TypedOpMap multiplyMap_;     //maps * ops
  TypedOpMap divideMap_;       //maps / ops


private:  //structors

  BinaryOpMapper();

  //generated destructor is fine


public:

  ElementalOperation const& Get(char& leftType, char rightType, char opSymbol) const;


private:  //implementation helpers

  //fetch op map for give symbol
  TypedOpMap const& ChooseMap(char opSymbol) const;

  //add an entry from given params in an op map
  static void Add(TypedOpMap& map, char result, char left, char right, ElementalOperation const& op);


public:  //sole instance

  static BinaryOpMapper const instance;

};


} } } //namespace avs::parser::action

#endif //__AVS_PARSER_ACTION_BINARYOPMAPPER_H__
