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


#ifndef __AVS_ROUND_H__
#define __AVS_ROUND_H__

//boost include
#include <boost/utility/enable_if.hpp>


namespace avs {



////////////////////////////////////////////////////////////////////////////////////
//  RoundUp<int Rounder, T>
//
//  small template helper function to Round up values (to powers of 2)
//
template <int Rounder, typename T> 
inline typename boost::enable_if_c<(Rounder & -Rounder) == Rounder, T>::type
RoundUp(T value)
{
  return (value + Rounder - 1) & -Rounder;
}


////////////////////////////////////////////////////////////////////////////////////
//  Round<int Rounder, T>
//
//  small template helper function to Round values (to powers of 2)
//
//  NB: note that Round / 2 is rounded up
//
template <int Rounder, typename T>
inline typename boost::enable_if_c<(Rounder & -Rounder) == Rounder, T>::type
Round(T value)
{
  return (value + (Rounder >> 1)) & -Rounder;
}


////////////////////////////////////////////////////////////////////////////////////
//  Round<int Rounder, T>
//
//  small template helper function to Round down values (to powers of 2)
//
template <int Rounder, typename T>
inline typename boost::enable_if_c<(Rounder & -Rounder) == Rounder, T>::type
RoundDown(T value)
{
  return value & -Rounder;
}


////////////////////////////////////////////////////////////////////////////////////////////
//  RoundingShift<int Shift, T>
//
//  
//
template <int Shift, typename T>
inline T
RoundingShift(T value)
{
  return ( value + (1 << (Shift - 1)) ) >> Shift;
}



} //namespace avs

#endif //__AVS_ROUND_H__
