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


//avisynth includes
#include "get.h"
#include "../adapt.h"
#include "binaryopmapper.h"
#include "../../core/exception/generic.h"


namespace avs { namespace parser { namespace action {



ElementalOperation const Get::equal_op[5] = 
    { adapt( std::equal_to<bool>() )
    , adapt( std::equal_to<PClip>() )
    , adapt( std::equal_to<double>() )
    , adapt( std::equal_to<int>() )
    , adapt( std::equal_to<std::string>() ) 
    }; 

ElementalOperation const Get::not_equal_op[5] = 
    { adapt( std::not_equal_to<bool>() )
    , adapt( std::not_equal_to<PClip>() )
    , adapt( std::not_equal_to<double>() )
    , adapt( std::not_equal_to<int>() )
    , adapt( std::not_equal_to<std::string>() ) 
    }; 


struct StringSubscript
{
  std::string operator()(std::string const& val, int begin, int end) const
  {
    if ( begin < 0 )
      begin = 0;
    if ( end < 0 )
      end = 0;
    return val.substring(begin, end);
  }
}

ElementalOperation const subscript_op = adapt( StringSubscript() );

ElementalOperation const& Get::SubscriptOperation(char type, bool firstArgOnly)
{

  switch( type )
  {
  case 's': if ( ! firstArgOnly )
              return subscript_op;

  default: throw exception::Generic("Illegal use of operator[]");
  }

}


ElementalOperation const& Get::EqualityOperation(char& leftType, char rightType, bool isEqual)
{
  if ( leftType != rightType )
    throw exception::Generic("Cannot compare expression of different types");
  if ( leftType == 'v' )
    throw exception::Generic("Cannot do comparison on void");

  char type = leftType;
  leftType = 'b';                                                //set result type to bool

  return (isEqual ? equal_op : not_equal_op)[ TypeToIndex(type) ];  //returns comparison op
}


ElementalOperation const& Get::BinaryOperation(char& leftType, char rightType, char opSymbol)
{
  return BinaryOpMapper::instance.Get(leftType, rightType, opSymbol);
}



int Get::TypeToIndex(char type)
{
  switch(type)
  {
  case 'b': return 0;
  case 'c': return 1;
  case 'f': return 2;
  case 'i': return 3;
  case 's': return 4;
  default: throw exception::Generic("Illegal type encountered");
  }
}






} } } //namespace avs::parser::action
