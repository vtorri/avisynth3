// Avisynth v3.0 alpha.  Copyright 2003 Ben Rudiak-Gould et al.
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


//avisynth include
#include "video.h"
#include "../../core/clip.h"
#include "../../core/exception.h"
#include "../../core/videoinfo.h"


namespace avs { namespace vfw { namespace avistream {



STDMETHODIMP Video::ReadFormat(LONG lPos, LPVOID lpFormat, LONG *lpcbFormat)
{
  if ( lpFormat == NULL )
  {
    *lpcbFormat = sizeof(BITMAPINFOHEADER);
	  return S_OK;
  }

  CPVideoInfo vi = GetVideoInfo();

  BITMAPINFOHEADER * bi = (BITMAPINFOHEADER *)lpFormat;
  memset(bi, 0, sizeof(BITMAPINFOHEADER));

  bi->biSize        = sizeof(BITMAPINFOHEADER);
  bi->biWidth       = vi->GetWidth();
  bi->biHeight      = vi->GetHeight();
  bi->biPlanes      = 1;
  bi->biBitCount    = vi->BitsPerPixel();
  bi->biCompression = vi->IsRGB() ? BI_RGB : fccHandler_;
  bi->biSizeImage   = GetBMPSize(vi->GetDimension());

  return S_OK;
}




STDMETHODIMP Video::Read(LONG lStart, LONG lSamples, LPVOID lpBuffer, LONG cbBuffer, LONG *plBytes, LONG *plSamples)
{
  CPVideoInfo vi = GetVideoInfo();

  int bmpSize = GetBMPSize(vi->GetDimension());
    
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
    catch (...) 
    {
      MakeErrorStream("Avisynth: unknown exception");
      ReadWrapper(lpBuffer, lStart, lSamples);
    }
  }
  catch (...) { return E_FAIL; }

  return S_OK;
}


void Video::FillAviStreamInfo(AVISTREAMINFOW& asi)
{ 
  CPVideoInfo vi = GetVideoInfo();

  int bmpSize = GetBMPSize(vi->GetDimension());

  asi.fccType        = streamtypeVIDEO;
  asi.dwQuality      = DWORD(-1);     
  asi.fccHandler     = fccHandler_;
  asi.dwScale        = vi->GetFPSDenominator();    
  asi.dwRate         = vi->GetFPSNumerator();
  asi.dwLength       = vi->GetFrameCount();
  asi.rcFrame.right  = vi->GetWidth();
  asi.rcFrame.bottom = vi->GetHeight();      
  asi.dwSampleSize   = bmpSize;
  asi.dwSuggestedBufferSize = bmpSize; 

  wcscpy(asi.szName, L"Avisynth video #1");  
}


void Video::Read(void* lpBuffer, int lStart, int lSamples)
{
  CPVideoFrame frame = GetClip()->GetFrame(lStart);
  ReadFrame(*frame, (BYTE *)lpBuffer);
}



} } } //namespace avs::vfw::avistream