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


#ifndef __AVS_EXCEPTION_NOSUCHPLANE_H__
#define __AVS_EXCEPTION_NOSUCHPLANE_H__

//avisynth includes
#include "../exception.h"
#include "../colorspace.h"

//boost include
#include <boost/format.hpp>


namespace avs { namespace exception {



//////////////////////////////////////////////////////////////////////////////////
//  NoSuchPlane
//
//  report that the given ColorSpace doesn't have the plane
//
class NoSuchPlane : public Exception
{

  char plane_;
  PColorSpace space_;


public:  //structors

  NoSuchPlane(PColorSpace const& space, char plane)
    : plane_( plane )
    , space_( space ) { }

  virtual ~NoSuchPlane() throw() { }


public:  //diagnotic message

  virtual std::string msg() const
  {
    return str( boost::format("%s doesn't have plane %c") % space_->GetName() % plane_ );
  }

};



} } //namespace avs::exception

#endif //__AVS_EXCEPTION_NOSUCHPLANE_H__