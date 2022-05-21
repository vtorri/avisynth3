// Avisynth v3.0 alpha.  Copyright 2003-2005 David Pierre - Ben Rudiak-Gould et al.
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
#include "colorbars.h"
#include "../../core/colorspace.h"
#include "../../core/colorspace/get.h"
#include "../../core/videoframe.h"
#include "../../core/cow_shared_ptr.h"
#include "../../core/runtime_environment.h"


namespace avs { namespace filters {



CPVideoFrame ColorBars::CreateFrame(Dimension const& dim, PEnvironment const& env)
{

  PVideoFrame result = env->CreateFrame(colorspace::Get::RGB32(), dim, PROGRESSIVE);

  window_ptr<int> dst = result->WriteTo('~');


  int y = 0;

  // these values are taken from http://www.greatdv.com/video/smptebars3.htm
  // note we go bottom->top
  static const int bottom_quarter[] = { 0x001d42, 0xebebeb, 0x2c005c, 0x101010,  0x070707, 0x101010, 0x181818,  0x101010 };

  for ( ; y < dim.GetHeight() / 4; ++y, dst.to(0, 1) )
  {
    int x = 0;
    int i = 0;

    for ( ; i < 4; ++i )
      for ( ; x < ( dim.GetWidth() * (i + 1) * 5 + 14) / 28; ++x )
        dst.ptr[x] = bottom_quarter[i];

    for ( ; i < 7; ++i )
      for ( ; x < ( dim.GetWidth() * (i + 12) + 10) / 21; ++x )
        dst.ptr[x] = bottom_quarter[i];

    for (; x < dim.GetWidth(); ++x)
      dst.ptr[x] = bottom_quarter[7];
  }


  static const int two_thirds_to_three_quarters[] = { 0x0f0fb4, 0x101010, 0xb410b4, 0x101010, 0x0db4b4, 0x101010, 0xb4b4b4 };

  for ( ; y < dim.GetHeight() / 3; ++y, dst.to(0, 1) )
  {
    int x = 0;

    for ( int i = 0; i < 7; ++i )
      for ( ; x < ( dim.GetWidth() * (i + 1) + 3 ) / 7; ++x )
        dst.ptr[x] = two_thirds_to_three_quarters[i];
  }


  static const int top_two_thirds[] = { 0xb4b4b4, 0xb4b40c, 0x0db4b4, 0x0db40c, 0xb410b4, 0xb40f0e, 0x0f0fb4 };

  for ( ; y < dim.GetHeight(); ++y, dst.to(0, 1) )
  {
    int x = 0;

    for (int i = 0; i < 7; ++i )
      for ( ; x < ( dim.GetWidth() * (i + 1) + 3) / 7; ++x )
        dst.ptr[x] = top_two_thirds[i];
  }


  return result;

}





} } //namespace avs::filters
