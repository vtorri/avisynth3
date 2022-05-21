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


#ifndef __AVS_PARSER_LAZY_TUPLE_H__
#define __AVS_PARSER_LAZY_TUPLE_H__

//boost includes
#include <boost/ref.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/type_traits/add_reference.hpp>

//spirit includes
#define PHOENIX_LIMIT 6
#include <boost/spirit/phoenix/functions.hpp>


namespace avs { namespace parser { namespace lazy {



///////////////////////////////////////////////////////////////////////////////
//  Attribute<int n>
//
//  accesses atribute n from a boost tuple which may be in a reference_wrapper
//  
template <int n> struct Attribute
{

  template <typename T, typename D = Attribute<n> >
  struct result
  {
    //NB: add_reference is necessary, in the case where the tuple element is a reference itself
    typedef typename boost::add_reference<typename boost::tuples::element<n, T>::type>::type type;
  };

  template <typename T, typename D>
  struct result<boost::reference_wrapper<T>, D>
  {
    typedef typename result<T>::type type;
  };

  template <typename T, typename D>
  struct result<boost::reference_wrapper<T> const, D>
  {
    typedef typename result<T>::type type;
  };

  //deflect phoenix::nil_t... no idea why it's necessary
  template <typename D>
  struct result<phoenix::nil_t, D>
  {
    typedef phoenix::nil_t type;
  };

  template <typename T>
  typename result<T>::type operator()(T& val) const
  { 
    return val.template get<n>();
  }

  template <typename T>
  typename result<T>::type operator()(boost::reference_wrapper<T> val) const
  {
    return val.get().template get<n>();
  }

};


phoenix::function<Attribute<0> > const first = Attribute<0>();
phoenix::function<Attribute<1> > const second = Attribute<1>();
phoenix::function<Attribute<2> > const third = Attribute<2>();



} } } //namespace avs::parser::lazy

#endif //__AVS_PARSER_LAZY_TUPLE_H__
