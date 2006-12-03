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


//avisynth includes
#include "convertaudio.h"
#include "saturate.h"


namespace avs { namespace filters {



void ConvertAudio::ToInt8::FromInt16::Convert(BYTE * output, BYTE * input_, int count) const
{
  short * input = (short *) input_;

  for( int i = count; i-- > 0; ++input, ++output )
    *output = (BYTE) ( (*input >> 8) + 128 );
}


void ConvertAudio::ToInt8::FromInt32::Convert(BYTE * output, BYTE * input_, int count) const
{
  int * input = (int *) input_;

  for( int i = count; i-- > 0; ++input, ++output )
    *output = (BYTE) ( (*input >> 24) + 128 );
}


void ConvertAudio::ToInt8::FromFloat::Convert(BYTE * output, BYTE * input_, int count) const
{
  float * input = (float *) input_;

  for( int i = count; i-- > 0; ++input, ++output )
    *output = Saturate<signed char>( int(*input * 128.0f) ) + 128;
}



void ConvertAudio::ToInt16::FromInt8::Convert(BYTE * output_, BYTE * input, int count) const
{

  short * output = (short *) output_;

  for( int i = count; i-- > 0; ++input, ++output )
    *output = short(*input - 128) << 8;
}


void ConvertAudio::ToInt16::FromInt32::Convert(BYTE * output_, BYTE * input_, int count) const
{

  short * output = (short *) output_;
  int * input = (int *) input_;

  for( int i = count; i-- > 0; ++input, ++output )
    *output = short( *input >> 16 );
}


void ConvertAudio::ToInt16::FromFloat::Convert(BYTE * output_, BYTE * input_, int count) const
{

  short * output = (short *) output_;
  float * input = (float *) input_;

  for( int i = count; i-- > 0; ++input, ++output )
    *output = Saturate<short>( int(*input * 32768.0f) );
}



void ConvertAudio::ToInt32::FromInt8::Convert(BYTE * output_, BYTE * input, int count) const
{

  int * output = (int *) output_;

  for( int i = count; i-- > 0; ++input, ++output )
    *output = int(*input - 128) << 24;
}


void ConvertAudio::ToInt32::FromInt16::Convert(BYTE * output_, BYTE * input_, int count) const
{

  int * output = (int *) output_;
  short * input = (short *) input_;

  for( int i = count; i-- > 0; ++input, ++output )
    *output = int(*input) << 16;
}

void ConvertAudio::ToInt32::FromFloat::Convert(BYTE * output_, BYTE * input_, int count) const
{

  int * output = (int *) output_;
  float * input = (float *) input_;

  for( int i = count; i-- > 0; ++input, ++output )
    *output = Saturate<int>(__int64( *input * std::numeric_limits<int>::max() ) );
}



void ConvertAudio::ToFloat::FromInt8::Convert(BYTE * output_, BYTE * input, int count) const
{

  float * output = (float *) output_;

  for( int i = count; i-- > 0; ++input, ++output )
    *output = float(*input) / 128.0f;
}


void ConvertAudio::ToFloat::FromInt16::Convert(BYTE * output_, BYTE * input_, int count) const
{

  float * output = (float *) output_;
  short * input = (short *) input_;

  for( int i = count; i-- > 0; ++input, ++output )
    *output = float(*input) / 32768.0f;
}


void ConvertAudio::ToFloat::FromInt16::Convert(BYTE * output_, BYTE * input_, int count) const
{

  float * output = (float *) output_;
  int * input = (int *) input_;

  for( int i = count; i-- > 0; ++input, ++output )
    *output = float(*input) / std::numeric_limits<int>::max();
}


} } //namespace avs::filters