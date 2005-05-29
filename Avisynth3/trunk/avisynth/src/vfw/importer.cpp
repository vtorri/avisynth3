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


#ifdef _WIN32

//avisynth includes
#include "bitmapinfoheader.h"
#include "../core/colorspace.h"
#include "importer/yv12.h"
#include "importer/interleaved.h"
#include "../core/utility/valuecache.h"


namespace avs { namespace vfw {



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

Importer const * CreateYV12Importer()
{
  return new importer::YV12();
}


utility::ValueCache<Importer const> rgb24( &CreateRGB24Importer );
utility::ValueCache<Importer const> rgb32( &CreateRGB32Importer );
utility::ValueCache<Importer const> yuy2( &CreateYUY2Importer );
utility::ValueCache<Importer const> yv12( &CreateYV12Importer );


} //namespace anonymous



PImporter Importer::Get(BitmapInfoHeader const& bih)
{
  switch( bih.biCompression )
  {
  case BI_RGB:
    switch ( bih.biBitCount )
    {
    case 32: return rgb32.Get();
    case 24: return rgb24.Get();
    default: break;
    } 
    break;

  case '2YUY': return yuy2.Get();
  case '21VY': return yv12.Get();
  default: break;
  }

  return PImporter();
}


} } //namespace avs::vfw

#endif //_WIN32
