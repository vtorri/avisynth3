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


#ifndef __AVS_PARSER_GRAMMAR_CHECK_H__
#define __AVS_PARSER_GRAMMAR_CHECK_H__

//boost include
#include <boost/optional.hpp>


namespace avs { namespace parser { namespace grammar {



struct Check
{

  //check if type allows infix notation, return it if yes, else throw
  static char IsInfixable(char type);  

  //check if type is typeExpected, if not throw
  static void TypeIsExpected(char type, char typeExpected);

  //if last is defined, return its value, else throw
  static int LastIsDefined(boost::optional<int> const& last);

  //check if return if the one expected, if not throw
  static void ReturnTypeIsExpected(char type, char typeExpected);

};


} } } //namespace avs::parser::grammar

#endif //__AVS_PARSER_GRAMMAR_CHECK_H__
