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


#ifndef __AVS_PARSER_ADAPTOR_DECAYER_H__
#define __AVS_PARSER_ADAPTOR_DECAYER_H__


namespace avs { namespace parser { namespace adaptor {



//////////////////////////////////////////////////////////////////////////
//  decayer<ThisMethodPtr> and its specialisations
//
//  decays a const member method to its basic function type
//
//  NB: this was necessary because matching through Function Type::*
//      keeps the const in the way (a VC++ bug ?)
//
template <typename ThisMethodPtr>
struct decayer { };


template <typename R, typename T>
struct decayer<R (T::*)() const>
{
  typedef R type();
};


template <typename R, typename T, typename A1>
struct decayer<R (T::*)(A1) const>
{
  typedef R type(A1);
};


template <typename R, typename T, typename A1, typename A2>
struct decayer<R (T::*)(A1, A2) const>
{
  typedef R type(A1, A2);
};


template <typename R, typename T, typename A1, typename A2, typename A3>
struct decayer<R (T::*)(A1, A2, A3) const>
{
  typedef R type(A1, A2, A3);
};


template <typename R, typename T, typename A1, typename A2, typename A3, typename A4>
struct decayer<R (T::*)(A1, A2, A3, A4) const>
{
  typedef R type(A1, A2, A3, A4);
};


template <typename R, typename T, typename A1, typename A2, typename A3, typename A4, typename A5>
struct decayer<R (T::*)(A1, A2, A3, A4, A5) const>
{
  typedef R type(A1, A2, A3, A4, A5);
};


template <typename R, typename T, typename A1, typename A2, typename A3, typename A4, typename A5
                                , typename A6>
struct decayer<R (T::*)(A1, A2, A3, A4, A5, A6) const>
{
  typedef R type(A1, A2, A3, A4, A5, A6);
};


template <typename R, typename T, typename A1, typename A2, typename A3, typename A4, typename A5
                                , typename A6, typename A7>
struct decayer<R (T::*)(A1, A2, A3, A4, A5, A6, A7) const>
{
  typedef R type(A1, A2, A3, A4, A5, A6, A7);
};


template <typename R, typename T, typename A1, typename A2, typename A3, typename A4, typename A5
                                , typename A6, typename A7, typename A8>
struct decayer<R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8) const>
{
  typedef R type(A1, A2, A3, A4, A5, A6, A7, A8);
};


template <typename R, typename T, typename A1, typename A2, typename A3, typename A4, typename A5
                                , typename A6, typename A7, typename A8, typename A9>
struct decayer<R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8, A9) const>
{
  typedef R type(A1, A2, A3, A4, A5, A6, A7, A8, A9);
};


template <typename R, typename T, typename A1, typename A2, typename A3, typename A4, typename A5
                                , typename A6, typename A7, typename A8, typename A9, typename A10>
struct decayer<R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10) const>
{
  typedef R type(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10);
};


template <typename R, typename T, typename A1, typename A2, typename A3, typename A4, typename A5
                                , typename A6, typename A7, typename A8, typename A9, typename A10>
                                , typename B1>
struct decayer<R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1) const>
{
  typedef R type(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1);
};


template <typename R, typename T, typename A1, typename A2, typename A3, typename A4, typename A5
                                , typename A6, typename A7, typename A8, typename A9, typename A10>
                                , typename B1, typename B2>
struct decayer<R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2) const>
{
  typedef R type(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2);
};


template <typename R, typename T, typename A1, typename A2, typename A3, typename A4, typename A5
                                , typename A6, typename A7, typename A8, typename A9, typename A10>
                                , typename B1, typename B2, typename B3>
struct decayer<R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2, B3) const>
{
  typedef R type(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2, B3);
};


template <typename R, typename T, typename A1, typename A2, typename A3, typename A4, typename A5
                                , typename A6, typename A7, typename A8, typename A9, typename A10>
                                , typename B1, typename B2, typename B3, typename B4>
struct decayer<R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2, B3, B4) const>
{
  typedef R type(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2, B3, B4);
};


template <typename R, typename T, typename A1, typename A2, typename A3, typename A4, typename A5
                                , typename A6, typename A7, typename A8, typename A9, typename A10>
                                , typename B1, typename B2, typename B3, typename B4, typename B5>
struct decayer<R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2, B3, B4, B5) const>
{
  typedef R type(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2, B3, B4, B5);
};


template <typename R, typename T, typename A1, typename A2, typename A3, typename A4, typename A5
                                , typename A6, typename A7, typename A8, typename A9, typename A10>
                                , typename B1, typename B2, typename B3, typename B4, typename B5
                                , typename B6>
struct decayer<R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2, B3, B4, B5, B6) const>
{
  typedef R type(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2, B3, B4, B5, B6);
};


template <typename R, typename T, typename A1, typename A2, typename A3, typename A4, typename A5
                                , typename A6, typename A7, typename A8, typename A9, typename A10>
                                , typename B1, typename B2, typename B3, typename B4, typename B5
                                , typename B6, typename B7>
struct decayer<R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2, B3, B4, B5, B6, B7) const>
{
  typedef R type(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2, B3, B4, B5, B6, B7);
};


template <typename R, typename T, typename A1, typename A2, typename A3, typename A4, typename A5
                                , typename A6, typename A7, typename A8, typename A9, typename A10>
                                , typename B1, typename B2, typename B3, typename B4, typename B5
                                , typename B6, typename B7, typename B8>
struct decayer<R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2, B3, B4, B5, B6, B7, B8) const>
{
  typedef R type(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2, B3, B4, B5, B6, B7, B8);
};


template <typename R, typename T, typename A1, typename A2, typename A3, typename A4, typename A5
                                , typename A6, typename A7, typename A8, typename A9, typename A10>
                                , typename B1, typename B2, typename B3, typename B4, typename B5
                                , typename B6, typename B7, typename B8, typename B9>
struct decayer<R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2, B3, B4, B5, B6, B7, B8, B9) const>
{
  typedef R type(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2, B3, B4, B5, B6, B7, B8, B9);
};


template <typename R, typename T, typename A1, typename A2, typename A3, typename A4, typename A5
                                , typename A6, typename A7, typename A8, typename A9, typename A10>
                                , typename B1, typename B2, typename B3, typename B4, typename B5
                                , typename B6, typename B7, typename B8, typename B9, typename B10>
struct decayer<R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10) const>
{
  typedef R type(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10);
};



} } } //namespace avs::parser::adaptor

#endif //__AVS_PARSER_ADAPTOR_DECAYER_H__
