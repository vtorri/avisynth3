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


#ifndef __AVS_PARSER_ADAPT_H__
#define __AVS_PARSER_ADAPT_H__

//avisynth includes
#include "forward.h"
#include "vmoperation.h"
#include "adaptor/caller.h"
#include "adaptor/adaptor.h"
#include "adaptor/decayer.h"


namespace avs { namespace parser {


namespace detail {



////////////////////////////////////////////////////////////////////////////////////
//  adapt(Functor const& functor, FunctorMethodPtr)
//
//  implementation helper for adapt(Functor const& functor)
//
template <typename Functor, typename FunctorMethodPtr>
typename adaptor::adaptor<typename adaptor::caller<Functor, typename adaptor::decayer<FunctorMethodPtr>::type> >
adapt(Functor const& functor, FunctorMethodPtr)
{
  return adaptor::adaptor<adaptor::caller<Functor, adaptor::decayer<FunctorMethodPtr>::type> >(functor);
}


} //namespace detail


/////////////////////////////////////////////////////////////////////////////////////////
//  adapt(Function * function)
//
//  wraps the appropriate adaptor around a free function
//
template <typename Function>
ElementalOperation adapt(Function * function)
{
  return adaptor::adaptor<adaptor::caller<Function *, Function> >(function);
}


/////////////////////////////////////////////////////////////////////////////////////////
//  adapt(Functor const& functor)
//
//  wraps the appropriate adaptor around the passed functor operator()
//
template <typename Functor>
ElementalOperation adapt(Functor const& functor)
{
  return detail::adapt(functor, &Functor::operator());
}


} } //namespace avs::parser

#endif //__AVS_PARSER_ADAPT_H__
