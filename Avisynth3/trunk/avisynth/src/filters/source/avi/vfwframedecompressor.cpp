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


#ifdef _WIN32

//avisynth includes
#include "vfwframedecompressor.h"
#include "../../../core/colorspace.h"
#include "../../../core/exception/generic.h"


namespace avs { namespace filters { namespace avisource {



char const * const VFWFrameDecompressor::preferredColorSpace_[4] = { "YV12", "YUY2", "RGB32", "RGB24" };


VFWFrameDecompressor::VFWFrameDecompressor(AviSource const& src, vfw::PBitmapInfoHeader const& bih, PColorSpace& space)
  : FrameDecompressor( src )
  , hic_( bih->biCompression )
  , input_( bih )
{
  output_.SetDimension(bih->GetDimension());   //init output bih dimension

  int i = 0;
  do { output_.SetColorSpace( ColorSpace::FromString(preferredColorSpace_[i++]) ); }   //test an output colorspace
  while( ! hic_.DecompressQuery(*input_, output_) && i < 4 );    //while conversion can't be done and more spaces to test

  if ( i == 4 )      //ie all color spaces tested without success
    throw exception::Generic("Decompressor couldn't do any requested output");

  hic_.DecompressBegin(*input_, output_);     //now ready the codec for decompression

  space = output_.GetColorSpace();            //report output colorspace to caller
}


VFWFrameDecompressor::~VFWFrameDecompressor()
{
  hic_.DecompressEnd();
}


OwnedBlock VFWFrameDecompressor::operator()(bool keyframe, bool preroll, OwnedBlock const& src, long bytesRead)
{
 
  input_->biSizeImage = bytesRead;  
  OwnedBlock dst( src.GetEnvironment(), output_.biSizeImage + Guard* 2, true );

  BYTE * srcPtr = src.get() + Guard;   //position where the data starts
  BYTE * dstPtr = dst.get() + Guard;   //idem

  hic_.Decompress(*input_, output_, srcPtr, dstPtr, keyframe, preroll);

  return dst;
}




} } } //namespace avs::filters::avisource

#endif //_WIN32
