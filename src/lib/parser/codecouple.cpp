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


//avisynth includes
#include "codecouple.h"
#include "functor/elemtostatement.h"


namespace avs { namespace parser {



CodeCouple& CodeCouple::operator+=(ElementalOperation const& op)
{
  if ( statCode_.empty() )
    before_ += op;
  else after_ += op;

  return *this;
}


CodeCouple& CodeCouple::operator-=(StatementOperation const& op)
{
  if ( ! after_.empty() )
  {
    statCode_ += functor::ElementalToStatement(after_);  //convert to statement code and accumulate
    after_.clear();                                      //reset to nothing
  }
  statCode_ += op;

  return *this;
}


CodeCouple& CodeCouple::operator+=(ElementalCode const& code)
{
  if ( statCode_.empty() )
    before_ += code;
  else after_ += code;

  return *this;
}


CodeCouple& CodeCouple::operator+=(StatementCode const& code)
{
  if ( ! after_.empty() )
  {
    statCode_ += functor::ElementalToStatement(after_);  //convert to statement code and accumulate
    after_.clear();                                      //reset to nothing
  }
  statCode_ += code;

  return *this;
}


CodeCouple& CodeCouple::operator+=(CodeCouple const& code)
{
  operator+=(code.before_);
  operator+=(code.statCode_);
  return operator+=(code.after_);
}


CodeCouple::operator StatementCode() const
{
  StatementCode result;

  if ( ! before_.empty() )
    result += functor::ElementalToStatement(before_);

  result += statCode_;

  if ( ! after_.empty() )
    result += functor::ElementalToStatement(after_);

  return result;
}

} } //namespace avs::parser
