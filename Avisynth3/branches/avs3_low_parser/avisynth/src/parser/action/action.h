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


#ifndef __AVS_PARSER_ACTION_ACTION_H__
#define __AVS_PARSER_ACTION_ACTION_H__

//avisynth includes
#include "../codecouple.h"
#include "../grammar/forward.h"


namespace avs { namespace parser { namespace action {


struct Action
{

  //typedefs to save some typing
  typedef grammar::VarTable VarTable;
  typedef grammar::LocalContext LocalContext;
  typedef grammar::value::Expression ExpressionValue;

  //action for the CreateVar statement
  static void CreateVarStatement(CodeCouple& code, LocalContext& localCtxt, ExpressionValue const& expr, VarTable& table, std::string const& name);

  //action for the expression statement
  static void ExprStatement(CodeCouple& code, LocalContext& localCtxt, ExpressionValue const& expr);


private:

  static void CleanOldLast(CodeCouple& code, LocalContext& localCtxt, ExpressionValue const& expr);

};


} } } //namespace avs::parser::action

#endif //__AVS_PARSER_ACTION_ACTION_H__
