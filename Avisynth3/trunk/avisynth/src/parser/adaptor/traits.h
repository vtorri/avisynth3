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


#ifndef __AVS_PARSER_ADAPTOR_TRAITS_H__
#define __AVS_PARSER_ADAPTOR_TRAITS_H__


namespace avs { namespace parser { namespace adaptor { namespace detail {



//////////////////////////////////////////////////////////////////////////////////////////////
//  traits<Function>
//
//  allows to deduce args and return type from a function type
//
template <typename Function>
struct traits { };


template <typename R>
struct traits<R ()>
{
  typedef R ResultType;

  enum { arity = 0 };
};


template <typename R, typename A1>
struct traits<R (A1)>
{
  typedef R ResultType;
  
  typedef A1 Arg1Type;

  enum { arity = 1 };
};


template <typename R, typename A1, typename A2>
struct traits<R (A1, A2)>
{
  typedef R ResultType;

  typedef A1 Arg1Type;
  typedef A2 Arg2Type;

  enum { arity = 2 };
};


template <typename R, typename A1, typename A2, typename A3>
struct traits<R (A1, A2, A3)>
{
  typedef R ResultType;
  
  typedef A1 Arg1Type;
  typedef A2 Arg2Type;
  typedef A3 Arg3Type;

  enum { arity = 3 };
};


template <typename R, typename A1, typename A2, typename A3, typename A4>
struct traits<R (A1, A2, A3, A4)>
{
  typedef R ResultType;

  typedef A1 Arg1Type;
  typedef A2 Arg2Type;
  typedef A3 Arg3Type;
  typedef A4 Arg4Type;

  enum { arity = 4 };
};


template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
struct traits<R (A1, A2, A3, A4, A5)>
{
  typedef R ResultType;

  typedef A1 Arg1Type;
  typedef A2 Arg2Type;
  typedef A3 Arg3Type;
  typedef A4 Arg4Type;
  typedef A5 Arg5Type;

  enum { arity = 5 };
};


template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5
                    , typename A6>
struct traits<R (A1, A2, A3, A4, A5, A6)>
{
  typedef R ResultType;

  typedef A1 Arg1Type;
  typedef A2 Arg2Type;
  typedef A3 Arg3Type;
  typedef A4 Arg4Type;
  typedef A5 Arg5Type;
  typedef A6 Arg6Type;

  enum { arity = 6 };
};


template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5
                    , typename A6, typename A7>
struct traits<R (A1, A2, A3, A4, A5, A6, A7)>
{
  typedef R ResultType;

  typedef A1 Arg1Type;
  typedef A2 Arg2Type;
  typedef A3 Arg3Type;
  typedef A4 Arg4Type;
  typedef A5 Arg5Type;
  typedef A6 Arg6Type;
  typedef A7 Arg7Type;

  enum { arity = 7 };
};


template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5
                    , typename A6, typename A7, typename A8>
struct traits<R (A1, A2, A3, A4, A5, A6, A7, A8)>
{
  typedef R ResultType;

  typedef A1 Arg1Type;
  typedef A2 Arg2Type;
  typedef A3 Arg3Type;
  typedef A4 Arg4Type;
  typedef A5 Arg5Type;
  typedef A6 Arg6Type;
  typedef A7 Arg7Type;
  typedef A8 Arg8Type;

  enum { arity = 8 };
};


template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5
                    , typename A6, typename A7, typename A8, typename A9>
struct traits<R (A1, A2, A3, A4, A5, A6, A7, A8, A9)>
{
  typedef R ResultType;

  typedef A1 Arg1Type;
  typedef A2 Arg2Type;
  typedef A3 Arg3Type;
  typedef A4 Arg4Type;
  typedef A5 Arg5Type;
  typedef A6 Arg6Type;
  typedef A7 Arg7Type;
  typedef A8 Arg8Type;
  typedef A9 Arg9Type;

  enum { arity = 9 };
};


template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5
                    , typename A6, typename A7, typename A8, typename A9, typename A10>
struct traits<R (A1, A2, A3, A4, A5, A6, A7, A8, A9, A10)>
{
  typedef R ResultType;

  typedef A1 Arg1Type;
  typedef A2 Arg2Type;
  typedef A3 Arg3Type;
  typedef A4 Arg4Type;
  typedef A5 Arg5Type;
  typedef A6 Arg6Type;
  typedef A7 Arg7Type;
  typedef A8 Arg8Type;
  typedef A9 Arg9Type;
  typedef A10 Arg10Type;

  enum { arity = 10 };
};


template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5
                    , typename A6, typename A7, typename A8, typename A9, typename A10
                    , typename B1>
struct traits<R (A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1)>
{
  typedef R ResultType;

  typedef A1 Arg1Type;
  typedef A2 Arg2Type;
  typedef A3 Arg3Type;
  typedef A4 Arg4Type;
  typedef A5 Arg5Type;
  typedef A6 Arg6Type;
  typedef A7 Arg7Type;
  typedef A8 Arg8Type;
  typedef A9 Arg9Type;
  typedef A10 Arg10Type;
  typedef B1 Arg11Type;

  enum { arity = 11 };
};


template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5
                    , typename A6, typename A7, typename A8, typename A9, typename A10
                    , typename B1, typename B2>
struct traits<R (A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2)>
{
  typedef R ResultType;

  typedef A1 Arg1Type;
  typedef A2 Arg2Type;
  typedef A3 Arg3Type;
  typedef A4 Arg4Type;
  typedef A5 Arg5Type;
  typedef A6 Arg6Type;
  typedef A7 Arg7Type;
  typedef A8 Arg8Type;
  typedef A9 Arg9Type;
  typedef A10 Arg10Type;
  typedef B1 Arg11Type;
  typedef B2 Arg12Type;

  enum { arity = 12 };
};


template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5
                    , typename A6, typename A7, typename A8, typename A9, typename A10
                    , typename B1, typename B2, typename B3>
struct traits<R (A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2, B3)>
{
  typedef R ResultType;

  typedef A1 Arg1Type;
  typedef A2 Arg2Type;
  typedef A3 Arg3Type;
  typedef A4 Arg4Type;
  typedef A5 Arg5Type;
  typedef A6 Arg6Type;
  typedef A7 Arg7Type;
  typedef A8 Arg8Type;
  typedef A9 Arg9Type;
  typedef A10 Arg10Type;
  typedef B1 Arg11Type;
  typedef B2 Arg12Type;
  typedef B3 Arg13Type;

  enum { arity = 13 };
};


template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5
                    , typename A6, typename A7, typename A8, typename A9, typename A10
                    , typename B1, typename B2, typename B3, typename B4>
struct traits<R (A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2, B3, B4)>
{
  typedef R ResultType;

  typedef A1 Arg1Type;
  typedef A2 Arg2Type;
  typedef A3 Arg3Type;
  typedef A4 Arg4Type;
  typedef A5 Arg5Type;
  typedef A6 Arg6Type;
  typedef A7 Arg7Type;
  typedef A8 Arg8Type;
  typedef A9 Arg9Type;
  typedef A10 Arg10Type;
  typedef B1 Arg11Type;
  typedef B2 Arg12Type;
  typedef B3 Arg13Type;
  typedef B4 Arg14Type;

  enum { arity = 14 };
};


template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5
                    , typename A6, typename A7, typename A8, typename A9, typename A10
                    , typename B1, typename B2, typename B3, typename B4, typename B5>
struct traits<R (A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2, B3, B4, B5)>
{
  typedef R ResultType;

  typedef A1 Arg1Type;
  typedef A2 Arg2Type;
  typedef A3 Arg3Type;
  typedef A4 Arg4Type;
  typedef A5 Arg5Type;
  typedef A6 Arg6Type;
  typedef A7 Arg7Type;
  typedef A8 Arg8Type;
  typedef A9 Arg9Type;
  typedef A10 Arg10Type;
  typedef B1 Arg11Type;
  typedef B2 Arg12Type;
  typedef B3 Arg13Type;
  typedef B4 Arg14Type;
  typedef B5 Arg15Type;

  enum { arity = 15 };
};


template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5
                    , typename A6, typename A7, typename A8, typename A9, typename A10
                    , typename B1, typename B2, typename B3, typename B4, typename B5
                    , typename B6>
struct traits<R (A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2, B3, B4, B5, B6)>
{
  typedef R ResultType;

  typedef A1 Arg1Type;
  typedef A2 Arg2Type;
  typedef A3 Arg3Type;
  typedef A4 Arg4Type;
  typedef A5 Arg5Type;
  typedef A6 Arg6Type;
  typedef A7 Arg7Type;
  typedef A8 Arg8Type;
  typedef A9 Arg9Type;
  typedef A10 Arg10Type;
  typedef B1 Arg11Type;
  typedef B2 Arg12Type;
  typedef B3 Arg13Type;
  typedef B4 Arg14Type;
  typedef B5 Arg15Type;
  typedef B6 Arg16Type;

  enum { arity = 16 };
};


template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5
                    , typename A6, typename A7, typename A8, typename A9, typename A10
                    , typename B1, typename B2, typename B3, typename B4, typename B5
                    , typename B6, typename B7>
struct traits<R (A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2, B3, B4, B5, B6, B7)>
{
  typedef R ResultType;

  typedef A1 Arg1Type;
  typedef A2 Arg2Type;
  typedef A3 Arg3Type;
  typedef A4 Arg4Type;
  typedef A5 Arg5Type;
  typedef A6 Arg6Type;
  typedef A7 Arg7Type;
  typedef A8 Arg8Type;
  typedef A9 Arg9Type;
  typedef A10 Arg10Type;
  typedef B1 Arg11Type;
  typedef B2 Arg12Type;
  typedef B3 Arg13Type;
  typedef B4 Arg14Type;
  typedef B5 Arg15Type;
  typedef B6 Arg16Type;
  typedef B7 Arg17Type;

  enum { arity = 17 };
};


template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5
                    , typename A6, typename A7, typename A8, typename A9, typename A10
                    , typename B1, typename B2, typename B3, typename B4, typename B5
                    , typename B6, typename B7, typename B8>
struct traits<R (A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2, B3, B4, B5, B6, B7, B8)>
{
  typedef R ResultType;

  typedef A1 Arg1Type;
  typedef A2 Arg2Type;
  typedef A3 Arg3Type;
  typedef A4 Arg4Type;
  typedef A5 Arg5Type;
  typedef A6 Arg6Type;
  typedef A7 Arg7Type;
  typedef A8 Arg8Type;
  typedef A9 Arg9Type;
  typedef A10 Arg10Type;
  typedef B1 Arg11Type;
  typedef B2 Arg12Type;
  typedef B3 Arg13Type;
  typedef B4 Arg14Type;
  typedef B5 Arg15Type;
  typedef B6 Arg16Type;
  typedef B7 Arg17Type;
  typedef B8 Arg18Type;

  enum { arity = 18 };
};


template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5
                    , typename A6, typename A7, typename A8, typename A9, typename A10
                    , typename B1, typename B2, typename B3, typename B4, typename B5
                    , typename B6, typename B7, typename B8, typename B9>
struct traits<R (A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2, B3, B4, B5, B6, B7, B8, B9)>
{
  typedef R ResultType;

  typedef A1 Arg1Type;
  typedef A2 Arg2Type;
  typedef A3 Arg3Type;
  typedef A4 Arg4Type;
  typedef A5 Arg5Type;
  typedef A6 Arg6Type;
  typedef A7 Arg7Type;
  typedef A8 Arg8Type;
  typedef A9 Arg9Type;
  typedef A10 Arg10Type;
  typedef B1 Arg11Type;
  typedef B2 Arg12Type;
  typedef B3 Arg13Type;
  typedef B4 Arg14Type;
  typedef B5 Arg15Type;
  typedef B6 Arg16Type;
  typedef B7 Arg17Type;
  typedef B8 Arg18Type;
  typedef B9 Arg18Type;

  enum { arity = 19 };
};


template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5
                    , typename A6, typename A7, typename A8, typename A9, typename A10
                    , typename B1, typename B2, typename B3, typename B4, typename B5
                    , typename B6, typename B7, typename B8, typename B9, typename B10>
struct traits<R (A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10)>
{
  typedef R ResultType;

  typedef A1 Arg1Type;
  typedef A2 Arg2Type;
  typedef A3 Arg3Type;
  typedef A4 Arg4Type;
  typedef A5 Arg5Type;
  typedef A6 Arg6Type;
  typedef A7 Arg7Type;
  typedef A8 Arg8Type;
  typedef A9 Arg9Type;
  typedef A10 Arg10Type;
  typedef B1 Arg11Type;
  typedef B2 Arg12Type;
  typedef B3 Arg13Type;
  typedef B4 Arg14Type;
  typedef B5 Arg15Type;
  typedef B6 Arg16Type;
  typedef B7 Arg17Type;
  typedef B8 Arg18Type;
  typedef B9 Arg18Type;
  typedef B10 Arg20Type;

  enum { arity = 20 };
};


} } } } //namespace avs::parser::adaptor::detail

#endif //__AVS_PARSER_ADAPTOR_TRAITS_H__
