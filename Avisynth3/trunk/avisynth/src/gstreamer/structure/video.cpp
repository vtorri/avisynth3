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
#include "video.h"
#include "../importer/yv12.h"
#include "../importer/i420.h"
#include "../importer/interleaved.h"
#include "../../core/videoinfo.h"
#include "../../core/colorspace.h"
#include "../../core/utility/valuecache.h"
#include "../../core/geometry/dimension.h"            //so Dimension is defined
#include "../../core/exception/colorspace/unknown.h"

//stl include
#include <string>


namespace avs { namespace gstreamer { namespace structure {



PImporter Video::SetVideoInfo(VideoInfo& vi) const
{
  PImporter importer = GetImporter();

  vi.AddVideo( importer->GetColorSpace()
             , Dimension(GetIntField("width"), GetIntField("height"))
             , 0
             , Fraction( static_cast<long>(GetDoubleField("framerate") * 1000000), 1000000 )
             );

  return importer;
}



namespace {


Importer const * CreateRGB24Importer()
{
  return new importer::Interleaved
      ( boost::dynamic_pointer_cast<cspace::Interleaved const>( ColorSpace::rgb24() ) 
      );
}

Importer const * CreateRGB32Importer()
{
  return new importer::Interleaved
      ( boost::dynamic_pointer_cast<cspace::Interleaved const>( ColorSpace::rgb32() ) 
      );
}

Importer const * CreateYUY2Importer()
{
  return new importer::Interleaved
      ( boost::dynamic_pointer_cast<cspace::Interleaved const>( ColorSpace::yuy2() ) 
      );
}

Importer const * CreateYV12Importer() { return new importer::YV12(); }
Importer const * CreateI420Importer() { return new importer::I420(); }


utility::ValueCache<Importer const> rgb24( &CreateRGB24Importer );
utility::ValueCache<Importer const> rgb32( &CreateRGB32Importer );
utility::ValueCache<Importer const> yuy2( &CreateYUY2Importer );
utility::ValueCache<Importer const> yv12( &CreateYV12Importer );
utility::ValueCache<Importer const> i420( &CreateI420Importer );


} //namespace anonymous



PImporter Video::GetImporter() const
{
  std::string name = GetName();
  
  if ( name == "video/x-raw-rgb" )
    switch ( GetIntField("bpp") )
    {
    case 24: return rgb24.Get();
    case 32: return rgb32.Get();
    default: break;
    }   
    
  if ( name == "video/x-raw-yuv" )
    switch ( GetFourCCField("format") )
	  {
	  case 'YV12': return yv12.Get();
	  case 'I420': return i420.Get();
    case 'YUY2': return yuy2.Get();
	  default: break;
	  }
  throw exception::cspace::Unknown();
}



} } } //namespace avs::gstreamer::structure

#endif //_WIN32
