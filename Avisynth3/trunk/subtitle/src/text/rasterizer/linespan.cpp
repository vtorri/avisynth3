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
#include "linespan.h"
#include "../../core/utility/round.h"


namespace avs { namespace text { namespace rasterizer {



void LineSpan::Realize(BYTE * ptr, int step) const
{
  ptr += step * (begin >> 3);              //move ptr to where the span starts
  long round = utility::RoundUp<8>(begin);

  if ( begin != round )                    //ie if begin is not a multiple of 8
  {
    if ( end <= round )                    //if the span does not even cross over the 8 pixel block boundary
    {
      *ptr += static_cast<BYTE>(end - begin);               //count what we have 
      return;                              //and done
    }
    *ptr += static_cast<BYTE>(round - begin);
    ptr += step;
  }

  //now handles all the 8 pixels block there are
  for ( int i = (end - round) >> 3; i-- > 0; ptr += step )
    *ptr += 8;

  BYTE rest = static_cast<BYTE>(end & 7);  //rest after all the 8-pixels blocks
  if ( rest != 0 )                         //if there is one
    *ptr += rest;                          //take care of it
}



bool LineSpan::RightMerge(iterator& beginIt, iterator endIt)
{
  if ( beginIt == endIt || end < beginIt->begin )  //if at end, or no overlap
    return false;                                  //reports no merging occured

  do
  {
    if ( end < beginIt->end )      //means *beginIt is not fully contained inside this, it exits on the right
    {
      end = beginIt->end;          //expand end to the merged limit
      ++beginIt;                   //point to next span
      break;                       //break since it won't merge anymore: source spans don't overlap
    }
  }
  while ( ++beginIt != endIt && beginIt->begin <= end );
  //while there is more spans which overlap

  return true;                     //reports some merging occured 
}


bool LineSpan::RightMergeThickened(iterator& beginIt, iterator endIt, long radius)
{
  if ( beginIt == endIt || end < beginIt->begin - radius )  //if at end, or no overlap
    return false;                                           //reports no merging occured

  do
  {
    if ( end < beginIt->end + radius )       //thickened span goes outside this on the right
      end = beginIt->end + radius;           //merge limit
    //NB: no trick like above here, thickened spans can overlap
  }
  while ( ++beginIt != endIt && beginIt->begin - radius <= end );
  //while there is overlapping spans

  return true;
}



} } } //namespace avs::text::rasterizer
