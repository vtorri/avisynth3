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


#ifndef __AVS_PARSER_GRAMMAR_FORWARD_H__
#define __AVS_PARSER_GRAMMAR_FORWARD_H__

//avisynth includes
#include "vartable.h"
#include "../function/table.h"

//boost include
#include <boost/tuple/tuple.hpp>


namespace avs { namespace parser { namespace grammar {


//typedefs
typedef boost::tuples::tuple<VarTable, function::Table> GlobalContext;
typedef boost::tuples::tuple<VarTable, int, boost::optional<int> > LocalContext;


namespace value {


//////////////////////////////////////////////////////////////////////////////////////
//  value::ElementalOpProxy
//
//  wrapper around ElementlOperation
//  necessary to prevent conflicts between Phoenix and ElementOperation::operator()
//
typedef boost::tuples::tuple<ElementalOperation> ElementalOpProxy;   


//////////////////////////////////////////////////////////////////////////////////////
//  value::Expression
//
//  return value of the Expression grammar
//
//  1st arg : expr generation code
//  2nd arg : expr type
//  3rd arg : flag reporting if the expr was a top level assignment
//  
typedef boost::tuples::tuple<ElementalCode, char, bool> Expression;  


} //namespace value


} } } //namespace avs::parser::grammar

#endif //__AVS_PARSER_GRAMMAR_FORWARD_H__
