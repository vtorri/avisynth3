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


//avisynth includes
#include "action.h"
#include "../vmcode.h"
#include "../functor/adaptor.h"
#include "../../core/Exception/generic.h"


namespace avs { namespace parser { namespace grammar {



ElementalOperation const& Action::GetSubscriptOperation(char type, bool firstArgOnly)
{

  switch( type )
  {
  case 's':

  default: throw exception::Generic("Illegal use of operator[]");
  }

}


ElementalOperation const& Action::GetEqualityOperation(char& leftType, char rightType, bool isEqual)
{
  if ( leftType != rightType )
    throw exception::Generic("Cannot compare expression of different types");
  if ( leftType == 'v' )
    throw exception::Generic("Cannot do comparison on void");

  char type = leftType;
  leftType = 'b';                                                //set result type to bool

  return (isEqual ? equal_op : differ_op)[ TypeToIndex(type) ];  //returns comparison op
}


int Action::TypeToIndex(char type)
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


template <typename T> inline bool equals(T left, T right) { return left == right; }
template <typename T> inline bool differs(T left, T right) { return left != right; }


ElementalOperation const Action::equal_op[5] = 
    { functor::adapt(&equals<bool>)
    , functor::adapt(&equals<PClip const&>)
    , functor::adapt(&equals<double>)
    , functor::adapt(&equals<int>)
    , functor::adapt(&equals<std::string const&>) 
    }; 

ElementalOperation const Action::differ_op[5] = 
    { functor::adapt(&differs<bool>)
    , functor::adapt(&differs<PClip const&>)
    , functor::adapt(&differs<double>)
    , functor::adapt(&differs<int>)
    , functor::adapt(&differs<std::string const&>) 
    }; 



} } } //namespace avs::parser::grammar
