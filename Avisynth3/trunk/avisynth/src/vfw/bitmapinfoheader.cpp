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
#include "bitmapinfoheader.h"
#include "../core/videoinfo.h"
#include "../core/colorspace.h"
#include "../filters/source/video/importer/vfwyv12.h"
#include "../filters/source/video/importer/interleaved.h"


namespace avs { namespace vfw {



BitmapInfoHeader::BitmapInfoHeader(VideoInfo const& vi)
{
  Init(vi.GetDimension());

  SetExporter( boost::static_pointer_cast<Exporter const>(vi.GetColorSpace()->GetExporter("VFW")) );
}


filters::source::video::PImporter BitmapInfoHeader::GetImporter() const
{
  using namespace filters::source::video::importer;

  switch( biCompression )
  {
  case '21VY': return VFWYV12::yv12.Get();
  case '2YUY': return Interleaved::YUY2.Get();
  case BI_RGB:
    switch( biBitCount )
    {
    case 24: return Interleaved::RGB24.Get();
    case 32: return Interleaved::RGB32.Get();    
    default: break;
    }
  default:break;
  }

  return filters::source::video::PImporter();   //reports no import possible
}


void BitmapInfoHeader::SetExporter(PExporter const& exporter)
{
  biBitCount    = exporter->GetBitsPerPixel();
  biCompression = exporter->GetFourCCHandler();
  biSizeImage   = exporter->GetBitmapSize(GetDimension());
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
