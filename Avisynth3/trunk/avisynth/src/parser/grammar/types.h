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
// General Public License cover the whole combination.


#ifndef __AVS_PARSER_GRAMMAR_TYPES_H__
#define __AVS_PARSER_GRAMMAR_TYPES_H__

//spirit include
#include <boost/spirit/symbols.hpp>

//STL include
#include <map>


namespace avs { namespace parser { namespace grammar {



////////////////////////////////////////////////////////////////////////////////
//  Types
//
//  symbol table mapping type string to their char value
//
class Types : public boost::spirit::symbols<char>
{

  std::map<char, char const *> map_;


private:  //structors

  Types();

  //generated destructor is fine


public:

  char const * GetString(char type) const;


private:  //implementation

  void AddType(char type, char const * string);


public:  //sole instance

  static Types const instance;

};


} } } //namespace avs::parser::grammar

#endif //__AVS_PARSER_GRAMMAR_TYPES_H__
