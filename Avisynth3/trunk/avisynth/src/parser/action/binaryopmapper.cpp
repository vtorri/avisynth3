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
#include "../adapt.h"
#include "binaryopmapper.h"
#include "../exception/failure.h"
#include "../../core/exception/generic.h"


namespace avs { namespace parser { namespace action {


// functors who do the work
template <typename R, typename T, typename U>
struct plus
{
  R operator()(T left, U right) const { return left + right; }
};

template <typename R, typename T, typename U>
struct minus
{
  R operator()(T left, U right) const { return left - right; }
};

template <typename R, typename T, typename U>
struct multiplies
{
  R operator()(T left, U right) const { return left * right; }
};

template <typename R, typename T, typename U>
struct divides
{
  R operator()(T left, U right) const { return left / right; }
};



BinaryOpMapper::BinaryOpMapper()
{

  Add(  plusMap_, 'i', 'i', 'i', adapt( plus<int, int, int>() )  );
  Add(  plusMap_, 'd', 'd', 'd', adapt( plus<double, double, double>() )  );
  Add(  plusMap_, 'd', 'i', 'd', adapt( plus<double, int, double>() )  );
  Add(  plusMap_, 'd', 'd', 'i', adapt( plus<double, double, int>() )  );
  Add(  plusMap_, 's', 's', 's', adapt( plus<std::string, std::string const&, std::string const&>() )  );

  Add(  minusMap_, 'i', 'i', 'i', adapt( minus<int, int, int>() )  );
  Add(  minusMap_, 'd', 'd', 'd', adapt( minus<double, double, double>() )  );
  Add(  minusMap_, 'd', 'i', 'd', adapt( minus<double, int, double>() )  );
  Add(  minusMap_, 'd', 'd', 'i', adapt( minus<double, double, int>() )  );

  Add(  multiplyMap_, 'i', 'i', 'i', adapt( multiplies<int, int, int>() )  );
  Add(  multiplyMap_, 'd', 'd', 'd', adapt( multiplies<double, double, double>() )  );
  Add(  multiplyMap_, 'd', 'i', 'd', adapt( multiplies<double, int, double>() )  );
  Add(  multiplyMap_, 'd', 'd', 'i', adapt( multiplies<double, double, int>() )  );

  Add(  divideMap_, 'i', 'i', 'i', adapt( divides<int, int, int>() )  );
  Add(  divideMap_, 'd', 'd', 'd', adapt( divides<double, double, double>() )  );
  Add(  divideMap_, 'd', 'i', 'd', adapt( divides<double, int, double>() )  );
  Add(  divideMap_, 'd', 'd', 'i', adapt( divides<double, double, int>() )  );

}


ElementalOperation const& BinaryOpMapper::Get(char& leftType, char rightType, char opSymbol) const
{
  TypedOpMap const& map = ChooseMap(opSymbol);

  TypedOpMap::const_iterator it = map.find( std::make_pair(leftType, rightType) );

  if ( it == map.end() )
    throw exception::Generic("parsing failed");

  leftType = it->second.second;
  return it->second.first;
}




BinaryOpMapper::TypedOpMap const& BinaryOpMapper::ChooseMap(char opSymbol) const
{
  switch(opSymbol)
  {
  case '+': return plusMap_;
  case '-': return minusMap_;
  case '*': return multiplyMap_;
  case '/': return divideMap_;

  default: throw exception::parser::Failure();
  }
}


void BinaryOpMapper::Add(TypedOpMap& map, char result, char left, char right, ElementalOperation const& op)
{
  map[ std::make_pair(left, right) ] = std::make_pair(op, result);
}


BinaryOpMapper const BinaryOpMapper::instance;


} } } //namespace avs::parser::action
