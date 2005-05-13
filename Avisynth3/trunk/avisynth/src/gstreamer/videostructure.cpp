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


#ifndef _WIN32

//avisynth includes
#include "videostructure.h"
#include "../core/videoinfo.h"
#include "../core/colorspace.h"
#include "../core/exception/colorspace/unknown.h"


namespace avs { namespace gstreamer {



PColorSpace VideoStructure::GetColorSpace() const
{
  std::string name = GetName();
  
  if ( name == "video/x-raw-rgb" )
    switch ( GetIntField("bpp") )
    {
    case 24: return ColorSpace::rgb24();
    case 32: return ColorSpace::rgb32();    
    default: break;
    }  
    
  if ( name == "video/x-raw-yuv" )
    return ColorSpace::FromFourCC( GetFourCCField("format") );

  throw exception::cspace::Unknown();
}


Dimension VideoStructure::GetDimension() const
{
  return Dimension(GetIntField("width"), GetIntField("height");
}


Fraction VideoStructure::GetFPS() const
{
  return Fraction( static_cast<long>(GetDoubleField("framerate") * 1000000), 1000000 );
}



} } //namespace avs::gstreamer

#endif //_WIN32
