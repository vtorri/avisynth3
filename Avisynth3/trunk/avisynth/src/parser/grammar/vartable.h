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


#ifndef __AVS_PARSER_GRAMMAR_VARTABLE_H__
#define __AVS_PARSER_GRAMMAR_VARTABLE_H__

//boost include
#include <boost/tuple/tuple.hpp>     //for tuple
#include <boost/spirit/symbols.hpp>  //for symbols

namespace spirit = boost::spirit;


namespace avs { namespace parser { namespace grammar {


//typedef
typedef boost::tuples::tuple<int, char> TypedIndex;   //associate an index to a type 



////////////////////////////////////////////////////////////////////////////////
//  VarTable
//
//  symbols table mapping var names to their type and position in the stack
//
class VarTable : public spirit::symbols<TypedIndex>
{

  int size_;       //NB: not equivalent to the number of symbols defined


public:  //structors

  VarTable()
    : size_( 0 ) { }

  VarTable(VarTable const& other)
    : spirit::symbols<TypedIndex>( other )
    , size_( other.size_ ) { }

  //generated destructor is fine


public:  //VarTable interface

  int DefineVar(std::string const& name, char type)
  {
    TypedIndex value(size_, type);

    if ( TypedIndex * index = spirit::find(*this, name.c_str()) )   //if already defined
      *index = value;                                               //we override value
    else add(name.c_str(), value);                                  //else we add it

    return size_++;
  }

  int size() const { return size_; }

};


} } } //namespace avs::parser::grammar

#endif //__AVS_PARSER_GRAMMAR_VARTABLE_H__
