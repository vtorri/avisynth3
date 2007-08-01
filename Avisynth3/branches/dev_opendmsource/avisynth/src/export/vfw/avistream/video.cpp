// Avisynth v3.0 alpha.  Copyright 2003-2006 David Pierre - Ben Rudiak-Gould et al.
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
#include "video.h"
#include "../exporter.h"
#include "../bitmapinfoheader.h"
#include "../../../core/exception.h"
#include "../../../core/videoinfo.h"
#include "../avistreaminfo.h"


namespace avs { namespace export_ { namespace vfw { namespace avistream {



STDMETHODIMP Video::ReadFormat(LONG /*lPos*/, LPVOID lpFormat, LONG *lpcbFormat)
{
  long size = *lpcbFormat;                        //save old size
  *lpcbFormat = sizeof(BITMAPINFOHEADER);         //update to used (needed) size

  if ( lpFormat == NULL )                         //case where it was just a size request
	  return S_OK;

  if ( size < sizeof(BITMAPINFOHEADER) )          //case where passed buffer is too small
    return AVIERR_BUFFERTOOSMALL;

  //creates a BitmapInfoHeader in the passed buffer
  BitmapInfoHeader * bi = static_cast<BitmapInfoHeader *>(lpFormat);
  new (bi) BitmapInfoHeader(GetVideoInfo().GetDimension(), GetExporter());

  return S_OK;
}




STDMETHODIMP Video::Read(LONG lStart, LONG lSamples, LPVOID lpBuffer, LONG cbBuffer, LONG *plBytes, LONG *plSamples)
{
  long frameSize = GetExporter().GetFrameSize(lStart);

  if ( plSamples != NULL )
    *plSamples = 1;
  if ( plBytes != NULL )
    *plBytes = frameSize;

  if ( lpBuffer == NULL )
    return S_OK;

  if ( cbBuffer < frameSize )
    return AVIERR_BUFFERTOOSMALL;


  try
  {
    try { GetExporter().ExportFrame( lStart, static_cast<BYTE *>(lpBuffer) );  }
    catch (avs::Exception& ex) { MakeErrorStream(ex.msg()); }
    catch (std::exception& ex) { MakeErrorStream(ex.what()); }

    //NB: MakeErrorStream changed exporter
    GetExporter().ExportFrame( lStart, static_cast<BYTE *>(lpBuffer) );
  }
  catch (std::exception&) { return E_FAIL; }

  return S_OK;
}



void Video::FillAviStreamInfo(AVISTREAMINFOW * psi)
{
  new (reinterpret_cast<AviStreamInfo::Video *>(psi)) AviStreamInfo::Video(GetVideoInfo(), GetExporter());
}

long Video::FindKeySample(long n, bool previous)
{
  return previous ? GetExporter().GetPreviousKeyFrame(n)
                  : GetExporter().GetNextKeyFrame(n);
}



} } } } //namespace avs::export::vfw::avistream


#endif //_WIN32
