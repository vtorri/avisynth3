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


#ifndef __AVS_PARSER_VISITOR_ADD_H__
#define __AVS_PARSER_VISITOR_ADD_H__

//avisynth includes
#include "../failure.h"
#include "../avsvalue.h"


namespace avs { namespace parser { namespace visitor {



struct Plus : public boost::static_visitor<AVSValue>
{

  template <typename T, typename U>
  AVSValue operator()(T left, U right) const
  {
    throw exception::parser::Failure();
  }

  template <>
  AVSValue operator()(int left, int right) const
  {
    return left + right;
  }

  template <>
  AVSValue operator()(int left, double right) const
  {
    return left + right;
  }

  template <>
  AVSValue operator()(double left, int right) const
  {
    return left + right;
  }


  template <>
  AVSValue operator()(double left, double right) const
  {
    return left + right;
  }

  template <>
  AVSValue operator()(std::string const& left, std::string const& right) const
  {
    return left + right;
  }

  static char const * types() { return "iiiidddiddddsss"; }

};




struct Minus : public boost::static_visitor<AVSValue>
{

  template <typename T, typename U>
  AVSValue operator()(T left, U right) const
  {
    throw exception::parser::Failure();
  }

  template <>
  AVSValue operator()(int left, int right) const
  {
    return left - right;
  }

  template <>
  AVSValue operator()(int left, double right) const
  {
    return left - right;
  }

  template <>
  AVSValue operator()(double left, int right) const
  {
    return left - right;
  }


  template <>
  AVSValue operator()(double left, double right) const
  {
    return left - right;
  }

  static char const * types() { return "iiiidddidddd"; }

};



} } } //namespace avs::parser::visitor

#endif //__AVS_PARSER_VISITOR_ADD_H__
