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


#ifndef __AVS_PARSER_LAZY_REF_H__
#define __AVS_PARSER_LAZY_REF_H__

//boost includes
#include <boost/ref.hpp>
#include <boost/spirit/phoenix/functions.hpp>


namespace avs { namespace parser { namespace lazy {



/////////////////////////////////////////////////////////////////////////
//  Reffer
//
//  functor from which is made the ref lazy function
//
struct Reffer
{

  template <typename T>
  struct result
  {
    typedef typename boost::reference_wrapper<T> type;
  };

  template <typename T>
  typename result<T>::type operator()(T& val) const
  {
    return boost::reference_wrapper<T>(val);
  }

};


/////////////////////////////////////////////////////////////////////////////
//  ConstReffer
//
//  functor from which is made the cref lazy function
//
struct ConstReffer
{

  template <typename T>
  struct result
  {
    typedef typename boost::reference_wrapper<T const> type;
  };

  template <typename T>
  typename result<T>::type operator()(T& val) const
  {
    return boost::reference_wrapper<T const>(val);
  }

};



/////////////////////////////////////////////////////////////////////////////////
//  ref and cref
//
//  lazy versions of boost::ref and boost::cref
//  ie wraps their args into a boost::reference_wrapper
//
phoenix::function<Reffer> const ref_ = Reffer();
phoenix::function<ConstReffer> const cref_ = ConstReffer();


} } } //namespace avs::parser::lazy

#endif //__AVS_PARSER_LAZY_REF_H__
