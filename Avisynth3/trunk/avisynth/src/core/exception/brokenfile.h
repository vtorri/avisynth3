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


#ifndef __AVS_EXCEPTION_BROKENHFILE_H__
#define __AVS_EXCEPTION_BROKENHFILE_H__

//avisynth include
#include "../exception.h"

//boost include
#include <boost/format.hpp>


namespace avs { namespace exception {



//////////////////////////////////////////////////////////////////////////////
//  exception::BrokenFile
//
//  the file is broken or in an invalid format
//
class BrokenFile
{

  std::string fileName_;


public:  //structors

  BrokenFile(std::string const& fileName)
    : fileName_( fileName ) { }

  virtual ~BrokenFile() throw() { }


public:  //diagnostic message

  virtual std::string msg() const
  {
    return str( boost::format("File %s is broken or in an invalid format") % fileName_ );
  }

  virtual char const * what() const throw() { return "File is broken or in an invalid fomat"; }

};


} } //namespace avs::exception

#endif //__AVS_EXCEPTION_BROKENHFILE_H__
