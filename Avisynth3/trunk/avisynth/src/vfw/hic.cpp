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


//avisynth include
#include "hic.h"

//assert include
#include <assert.h>


namespace avs { namespace vfw {



Hic::Hic(DWORD fourcc)
  : hic_( ICOpen(ICTYPE_VIDEO, fourcc, ICMODE_DECOMPRESS), &ICClose )
  , ex_( false ) { }


Hic::Hic(BitmapInfoHeader const& bih)
  : hic_( ICLocate(ICTYPE_VIDEO, bih.biCompression, const_cast<BitmapInfoHeader *>(&bih), NULL, ICMODE_DECOMPRESS), &ICClose )
  , ex_( false ) { }


bool Hic::DecompressQuery(BitmapInfoHeader const& input, BitmapInfoHeader const& output) const
{
  return ICDecompressQuery(hic_.get(), const_cast<BitmapInfoHeader *>(&input), const_cast<BitmapInfoHeader *>(&output)) == ICERR_OK;
}


void Hic::DecompressBegin(BitmapInfoHeader const& input, BitmapInfoHeader const& output)
{
  LRESULT result 
      = ICDecompressBegin(hic_.get(), const_cast<BitmapInfoHeader *>(&input), const_cast<BitmapInfoHeader *>(&output));
    
  if ( result == ICERR_UNSUPPORTED )
  {
    ex_ = true;
    result = ICDecompressExBegin(hic_.get(), 0,
        const_cast<BitmapInfoHeader *>(&input), 0, 0, 0, input.biWidth, input.biHeight,
        const_cast<BitmapInfoHeader *>(&output), 0, 0, 0, output.biWidth, output.biHeight);
  }
  else
    ex_ = false;

  assert( result == ICERR_OK );
}


void Hic::DecompressEnd()
{
  ex_ ? ICDecompressExEnd(hic_.get())
      : ICDecompressEnd(hic_.get());
}


void Hic::Decompress(BitmapInfoHeader const& input, BitmapInfoHeader const& output, BYTE const * src, BYTE * dst, bool keyframe, bool preroll) const
{
  int flags = keyframe ? 0 : ICDECOMPRESS_NOTKEYFRAME;
  flags |= preroll ? ICDECOMPRESS_PREROLL : 0;

  LRESULT result
      = ex_ ? ICDecompressEx(hic_.get(), flags, const_cast<BitmapInfoHeader *>(&input), const_cast<BYTE *>(src), 0, 0, output.biWidth, output.biHeight
                                              , const_cast<BitmapInfoHeader *>(&output), dst, 0, 0, output.biWidth, output.biHeight)
            : ICDecompress(hic_.get(), flags, const_cast<BitmapInfoHeader *>(&input), const_cast<BYTE *>(src), const_cast<BitmapInfoHeader *>(&output), dst);

  assert( result == ICERR_OK );
}



} } //namespace avs::vfw
