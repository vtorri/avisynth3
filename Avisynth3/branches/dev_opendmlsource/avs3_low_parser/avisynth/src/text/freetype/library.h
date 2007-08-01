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


#ifndef __AVS_TEXT_FREETYPE_LIBRARY_H__
#define __AVS_TEXT_FREETYPE_LIBRARY_H__

//boost include
#include <boost/noncopyable.hpp>


//declarations
struct FT_LibraryRec_;
typedef FT_LibraryRec_ * FT_Library;


namespace avs { namespace text { namespace freetype {



/////////////////////////////////////////////////////////////////////////////
//  freetype::Library
//
//  C++ wrapper around freetype FT_Library handle
//
class Library : public boost::noncopyable
{

  FT_Library library_;  //underlying handle


private:  //structors
    
  Library();
  ~Library();


public:

  operator FT_Library() const { return library_; }


public:  //sole library instance

  static Library const instance;
    
};



} } } // namespace avs::text::freetype

#endif //__AVS_TEXT_FREETYPE_LIBRARY_H__
