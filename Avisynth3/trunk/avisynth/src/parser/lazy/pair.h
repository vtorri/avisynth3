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


#ifndef __AVS_PARSER_LAZY_PAIR_H__
#define __AVS_PARSER_LAZY_PAIR_H__

//boost includes
#include <boost/ref.hpp>
#include <boost/spirit/phoenix/functions.hpp>


namespace avs { namespace parser { namespace lazy {



struct FirstAttribute
{

  template <typename T>
  struct result
  {
    typedef typename T::first_type& type;
  };

  template <typename T>
  struct result<T const>
  {
    typedef typename T::first_type const& type;
  };

  template <typename T>
  struct result<boost::reference_wrapper<T> >
  {
    typedef typename result<T>::type type;
  };

  template <typename T>
  struct result<boost::reference_wrapper<T> const>
  {
    typedef typename result<T>::type type;
  };

  //deflect phoenix::nil_t... I dunno why it's necessary
  template <>
  struct result<phoenix::nil_t>
  {
    typedef phoenix::nil_t type;
  };

  template <typename T>
  typename result<T>::type operator()(T& val) const
  { 
    return val.first;
  }

  template <typename T>
  typename result<T>::type operator()(boost::reference_wrapper<T> val) const
  {
    return val.get().first;
  }

};



struct SecondAttribute
{

  template <typename T>
  struct result
  {
    typedef typename T::second_type& type;
  };

  template <typename T>
  struct result<T const>
  {
    typedef typename T::second_type const& type;
  };

  template <typename T>
  struct result<boost::reference_wrapper<T> >
  {
    typedef typename result<T>::type type;
  };

  template <typename T>
  struct result<boost::reference_wrapper<T> const>
  {
    typedef typename result<T>::type type;
  };

  //deflect phoenix::nil_t... I dunno why it's necessary
  template <>
  struct result<phoenix::nil_t>
  {
    typedef phoenix::nil_t type;
  };

  template <typename T>
  typename result<T>::type operator()(T& val) const
  { 
    return val.second;
  }

  template <typename T>
  typename result<T>::type operator()(boost::reference_wrapper<T> val) const
  {
    return val.get().second;
  }

};


phoenix::function<FirstAttribute> const first = FirstAttribute();
phoenix::function<SecondAttribute> const second = SecondAttribute();


} } } //namespace avs::parser::lazy

#endif //__AVS_PARSER_LAZY_PAIR_H__
