// Avisynth v3.0 alpha.  Copyright 2005 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_PARSER_ADAPTOR_ARG_H__
#define __AVS_PARSER_ADAPTOR_ARG_H__

//avisynth include
#include "traits.h"


namespace avs { namespace parser { namespace adaptor { namespace detail {



/////////////////////////////////////////////////////////////////////////////
//  arg<Function, int n> and its specialisations
//
//  extracts type of arg n of given function
//
template <typename Function, int n>
struct arg { };


template <typename Function>
struct arg<Function, 1>
{
  typedef typename traits<Function>::Arg1Type type;
};

template <typename Function>
struct arg<Function, 2>
{
  typedef typename traits<Function>::Arg2Type type;
};

template <typename Function>
struct arg<Function, 3>
{
  typedef typename traits<Function>::Arg3Type type;
};

template <typename Function>
struct arg<Function, 4>
{
  typedef typename traits<Function>::Arg4Type type;
};

template <typename Function>
struct arg<Function, 5>
{
  typedef typename traits<Function>::Arg5Type type;
};

template <typename Function>
struct arg<Function, 6>
{
  typedef typename traits<Function>::Arg6Type type;
};

template <typename Function>
struct arg<Function, 7>
{
  typedef typename traits<Function>::Arg7Type type;
};

template <typename Function>
struct arg<Function, 8>
{
  typedef typename traits<Function>::Arg8Type type;
};

template <typename Function>
struct arg<Function, 9>
{
  typedef typename traits<Function>::Arg9Type type;
};

template <typename Function>
struct arg<Function, 10>
{
  typedef typename traits<Function>::Arg10Type type;
};

template <typename Function>
struct arg<Function, 11>
{
  typedef typename traits<Function>::Arg11Type type;
};

template <typename Function>
struct arg<Function, 12>
{
  typedef typename traits<Function>::Arg12Type type;
};

template <typename Function>
struct arg<Function, 13>
{
  typedef typename traits<Function>::Arg13Type type;
};

template <typename Function>
struct arg<Function, 14>
{
  typedef typename traits<Function>::Arg14Type type;
};

template <typename Function>
struct arg<Function, 15>
{
  typedef typename traits<Function>::Arg15Type type;
};

template <typename Function>
struct arg<Function, 16>
{
  typedef typename traits<Function>::Arg16Type type;
};

template <typename Function>
struct arg<Function, 17>
{
  typedef typename traits<Function>::Arg17Type type;
};

template <typename Function>
struct arg<Function, 18>
{
  typedef typename traits<Function>::Arg18Type type;
};

template <typename Function>
struct arg<Function, 19>
{
  typedef typename traits<Function>::Arg19Type type;
};

template <typename Function>
struct arg<Function, 20>
{
  typedef typename traits<Function>::Arg20Type type;
};



} } } } //namespace avs::parser::adaptor::detail

#endif //__AVS_PARSER_ADAPTOR_ARG_H__
