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


//avisynth includes
#include "video.h"
#include "../../core/clip.h"
#include "../avistreaminfo.h"
#include "../bitmapinfoheader.h"
#include "../../core/exception.h"
#include "../../core/videoinfo.h"
#include "../../core/colorspace.h"


namespace avs { namespace vfw { namespace avistream {



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
  new (bi) BitmapInfoHeader(*GetVideoInfo());

  return S_OK;
}




STDMETHODIMP Video::Read(LONG lStart, LONG lSamples, LPVOID lpBuffer, LONG cbBuffer, LONG *plBytes, LONG *plSamples)
{
  CPVideoInfo vi = GetVideoInfo();

  int bmpSize = vi->GetColorSpace()->GetBitmapSize(vi->GetDimension());
    
  if ( plSamples != NULL )
    *plSamples = 1;
  if ( plBytes != NULL ) 
    *plBytes = bmpSize;

  if ( lpBuffer == NULL )
    return S_OK;

  if ( cbBuffer < bmpSize )
    return AVIERR_BUFFERTOOSMALL;


  try 
  {
    try { ReadWrapper(lpBuffer, lStart, lSamples); }
    catch (avs::Exception& ex)
    {
      MakeErrorStream(ex.msg());
      ReadWrapper(lpBuffer, lStart, lSamples);
    }
 /*   catch (...) 
    {
      MakeErrorStream("Avisynth: unknown exception");
      ReadWrapper(lpBuffer, lStart, lSamples);
    }*/
  }
  catch (...) { return E_FAIL; }

  return S_OK;
}


void Video::FillAviStreamInfo(AviStreamInfo * asi)
{ 
  //constructs a video AviStreamInfo in the passed buffer
  new (asi) AviStreamInfo(*GetVideoInfo(), true);
}



void Video::Read(void* lpBuffer, int lStart, int /*lSamples*/)
{
  CPVideoFrame frame = GetClip()->GetFrame(lStart);
  ReadFrame(*frame, static_cast<BYTE *>(lpBuffer));
}



} } } //namespace avs::vfw::avistream
