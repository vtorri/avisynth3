// Avisynth v3.0 alpha.  Copyright 2003-2006 David Pierre - Ben Rudiak-Gould et al.
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


//avisynth includes
#include "operators.h"
#include "../virtualmachine.h"

//boost includes
#include <boost/variant/get.hpp>


namespace avs { namespace parser { namespace lowlvl {


// functors who do the work
template <typename T>
struct plus_equal
{
  void operator()(VirtualMachine& vm) const 
  { 
    boost::get<T>(vm.stack[1]) += boost::get<T>(vm.stack[0]);
    vm.stack.pop_front();
  }
};

template <typename T>
struct minus_equal
{
  void operator()(VirtualMachine& vm) const 
  { 
    boost::get<T>(vm.stack[1]) -= boost::get<T>(vm.stack[0]);
    vm.stack.pop_front();
  }
};

template <typename T>
struct multiplies_equal
{
  void operator()(VirtualMachine& vm) const 
  { 
    boost::get<T>(vm.stack[1]) *= boost::get<T>(vm.stack[0]);
    vm.stack.pop_front();
  }
};

template <typename T>
struct divides_equal
{
  void operator()(VirtualMachine& vm) const 
  { 
    boost::get<T>(vm.stack[1]) /= boost::get<T>(vm.stack[0]);
    vm.stack.pop_front();
  }
};


Operators::Operators()
{
  add( "+[ii]", Operation(plus_equal<int>()) );
  add( "+[dd]", Operation(plus_equal<double>()) );
  add( "+[ss]", Operation(plus_equal<std::string>()) );

  add( "-[ii]", Operation(minus_equal<int>()) );
  add( "-[dd]", Operation(minus_equal<double>()) );

  add( "*[ii]", Operation(multiplies_equal<int>()) );
  add( "*[dd]", Operation(multiplies_equal<double>()) );

  add( "/[ii]", Operation(divides_equal<int>()) );
  add( "/[dd]", Operation(divides_equal<double>()) );

}


} } } //namespace avs::parser::lowlvl