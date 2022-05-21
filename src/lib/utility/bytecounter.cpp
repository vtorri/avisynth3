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


//avisynth include
#include "bytecounter.h"

//stl include
#include <numeric>           //for std::accumulate


namespace avs {



int ByteCounter::GetMin() const
{
  int result = 0;

  while ( count_[result] == 0 && result < 255 ) ++result;

  return result;
}


int ByteCounter::GetMax() const
{
  int result = 255;

  while ( count_[result] == 0 && result > 0 ) --result;

  return result;
}



int ByteCounter::GetLooseMin(long limit) const
{
  long sum = 0;

  for ( int i = 0; i < 255; ++i )
    if ( (sum += count_[i]) > limit )
      return i;

  return 255;
}


int ByteCounter::GetLooseMax(long limit) const
{
  long sum = 0;

  for ( int i = 256; i-- > 0; )
    if ( (sum += count_[i]) > limit )
      return i;

  return 0;
}



long ByteCounter::GetTotal() const
{
  return std::accumulate( count_, count_ + 256, 0L );
}


long long ByteCounter::GetWeightedTotal() const
{
  long long result = 0;

  for ( int i = 256; i-- > 0; )
    result += count_[i] * i;

  return result;
}


} //namespace avs
