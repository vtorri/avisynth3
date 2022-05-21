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


#ifndef __AVS_EXCEPTION_COLORSPACE_UNSUPPORTED_H__
#define __AVS_EXCEPTION_COLORSPACE_UNSUPPORTED_H__

//avisynth includes
#include "../../exception.h"
#include "../../colorspace.h"

//boost include
#include <boost/format.hpp>


namespace avs { namespace exception { namespace colorspace {



//////////////////////////////////////////////////////////////////////////////////////
//  cspace::Unsupported
//
//  reports that the given ColorSpace is not supported by the attempted operation
//
class Unsupported : public Exception
{

  PColorSpace space_;


public:  //structors

  Unsupported(PColorSpace const& space)
    : space_( space ) { }

  virtual ~Unsupported() throw() { }


public:  //diagnotic message

  virtual std::string msg() const
  {
    return str( boost::format("%s is not supported") % space_->GetName() );
  }

};




} } } //namespace avs::exception::colorspace

#endif //__AVS_EXCEPTION_COLORSPACE_UNSUPPORTED_H__
