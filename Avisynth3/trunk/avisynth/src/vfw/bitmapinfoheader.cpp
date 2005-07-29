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


//avisynth includes
#include "exporter.h"
#include "bitmapinfoheader.h"
#include "../core/videoinfo.h"
#include "../core/colorspace/get.h"
#include "../core/utility/valuecache.h"
#include "../core/utility/synchronizer/lock.h"
#include "../core/colorspace/importer/planar_yuv.h"
#include "../core/colorspace/importer/interleaved.h"


namespace avs { namespace vfw {



namespace {


using namespace colorspace;

Importer const * CreateRGB24Importer() { return new importer::interleaved<4>(Get::RGB24()); }
Importer const * CreateRGB32Importer() { return new importer::interleaved<4>(Get::RGB32()); }
Importer const * CreateYUY2Importer() { return new importer::interleaved<4>(Get::YUY2()); }
Importer const * CreateYV12Importer() { return new importer::planar_yuv<4, 2>(Get::YV12()); }


typedef utility::value_cache<Importer const, utility::synchronizer::lock<Importer> > ImporterCache;

ImporterCache const rgb24(&CreateRGB24Importer);
ImporterCache const rgb32(&CreateRGB32Importer);
ImporterCache const yuy2(&CreateYUY2Importer);
ImporterCache const yv12(&CreateYV12Importer);


} //namespace anonymous



PImporter BitmapInfoHeader::GetImporter() const
{

  switch( biCompression )
  {
  case '21VY': return yv12.Get();
  case '2YUY': return yuy2.Get();
  case BI_RGB:
    switch( biBitCount )
    {
    case 24: return rgb24.Get();
    case 32: return rgb32.Get();    
    default: break;
    }
  default:break;
  }

  return PImporter();   //reports no import possible
}


void BitmapInfoHeader::SetExporter(Exporter const& exporter)
{
  biBitCount    = exporter.GetBitsPerPixel();
  biCompression = exporter.GetFourCCHandler();
  biSizeImage   = exporter.GetBitmapSize(GetDimension());
}


void BitmapInfoHeader::Init(Dimension const& dim)
{
  memset( static_cast<BITMAPINFOHEADER *>(this), 0, sizeof(BITMAPINFOHEADER) );

  biSize        = sizeof(BITMAPINFOHEADER);
  biPlanes      = 1;

  biWidth       = dim.GetWidth();
  biHeight      = dim.GetHeight();
}





} } //namespace avs::vfw
