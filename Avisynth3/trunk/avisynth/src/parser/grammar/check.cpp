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
#include "check.h"
#include "../exception/badreturntype.h"
#include "../../core/exception/generic.h"


namespace avs { namespace parser { namespace grammar {



char Check::IsInfixable(char type)
{
  if ( type != 'c' && type != 's' )
    throw exception::Generic("Illegal use of infix notation");

  return type;
}


void Check::TypeIsExpected(char type, char typeExpected)
{
  if ( type != typeExpected )
    throw exception::Generic("Type of expression is unexpected");
}


int Check::LastIsDefined(boost::optional<int> const& last)
{
  if ( ! last )
    throw exception::Generic("last was not defined");

  return last.get();
}


void Check::ReturnTypeIsExpected(char type, char typeExpected)
{
  if ( type != typeExpected )
    throw exception::parser::BadReturnType(typeExpected);
}

} } } //namespace avs::parser::grammar
