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


#ifndef __AVS_COLORSPACE_IMPL_INTERNALCREATOR_H__
#define __AVS_COLORSPACE_IMPL_INTERNALCREATOR_H__

//avisynth include
#include "../../forward.h"      //for PColorSpace, WeakPColorSpace

//boost include
#include <boost/weak_ptr.hpp>


namespace avs { namespace cspace { namespace impl {



//////////////////////////////////////////////////////////////////////////////////////////
//  Creator
//
//  envelops knowledge for creating an internal colorspace
//  while caching the value at the same time
//
//  NB: this class is not synchronized because it is provided by the Map class
//
class InternalCreator
{

  typedef ColorSpace * (* CreateMethod)();

  mutable WeakPColorSpace value_;
  CreateMethod create_;
  

public:  //structors

  InternalCreator(CreateMethod create)
    : create_( create ) { }

  //generated copy constructor and destructor are fine


public:  //interface

  PColorSpace operator()() const
  {
    PColorSpace result = value_.lock();
    if ( ! result )
      value_ = result = PColorSpace( create_() );
    return result;
  }

};


} } } //namespace avs::cspace::impl

#endif //__AVS_COLORSPACE_IMPL_INTERNALCREATOR_H__