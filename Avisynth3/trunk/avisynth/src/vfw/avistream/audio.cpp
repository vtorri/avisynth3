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
#include "audio.h"
#include "../avistreaminfo.h"
#include "../../core/clip.h"
#include "../../core/exception.h"
#include "../../core/videoinfo.h"

#ifndef _MSC_VER
#define AVISTREAMREAD_CONVENIENT  -1
#endif


namespace avs { namespace vfw { namespace avistream {



STDMETHODIMP Audio::ReadFormat(LONG /*lPos*/, LPVOID lpFormat, LONG *lpcbFormat)
{
  if ( lpFormat == NULL )
  {
    *lpcbFormat = sizeof(WAVEFORMATEX);
	  return S_OK;
  }

  CPVideoInfo vi = GetVideoInfo();

  WAVEFORMATEX * wfx = (WAVEFORMATEX *)lpFormat;
  memset(wfx, 0, sizeof(WAVEFORMATEX));

  wfx->wFormatTag      = 1;
  wfx->nChannels       = vi->GetChannelCount();  // Perhaps max out at 2?
  wfx->nSamplesPerSec  = vi->GetSampleRate();
  wfx->wBitsPerSample  = vi->BytesPerChannelSample() * 8;
  wfx->nBlockAlign     = vi->BytesPerAudioSample();
  wfx->nAvgBytesPerSec = wfx->nSamplesPerSec * wfx->nBlockAlign;
    
  return S_OK;
}


STDMETHODIMP Audio::Read(LONG lStart, LONG lSamples, LPVOID lpBuffer, LONG cbBuffer, LONG *plBytes, LONG *plSamples)
{
  CPVideoInfo vi = GetVideoInfo();

  if ( lSamples == AVISTREAMREAD_CONVENIENT )
    lSamples = static_cast<long>(vi->AudioSamplesFromFrames(1));
    
  if ( plBytes != NULL ) 
    *plBytes = vi->BytesPerAudioSample() * lSamples;
    
  if ( plSamples != NULL ) 
    *plSamples = lSamples;
    
  if ( lpBuffer == NULL )   //case when caller wanted to know buffer size needed
    return S_OK;

  if ( cbBuffer < *plBytes )
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


void Audio::FillAviStreamInfo(AviStreamInfo * asi)
{ 
  //creates an audio AviStreamInfo in the passed buffer
  new (asi) AviStreamInfo(*GetVideoInfo(), false);
}


void Audio::Read(void* lpBuffer, int lStart, int lSamples)
{
  GetClip()->GetAudio(static_cast<BYTE *>(lpBuffer), lStart, lSamples);
}


} } } //namespace avs::vfw::avistream
