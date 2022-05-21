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
#include "analyze.h"
#include "../../../core/videoframe.h"
#include "../../../utility/bytecounter.h"


namespace avs { namespace filters { namespace coloryuv {



PClip Analyze::clone(PClip const& child) const
{
  return PClip( static_cast<Clip *>(new Analyze(child)) );
}


CPVideoFrame Analyze::MakeFrame(PVideoFrame const& source) const
{

  ByteCounter y, u, v;

  Count(*source, y, u, v);

  long yPixelCount = y.GetTotal();
  long uPixelCount = u.GetTotal();
  long vPixelCount = v.GetTotal();

  long yLooseLimit = yPixelCount >> 8;
  long uLooseLimit = uPixelCount >> 8;
  long vLooseLimit = vPixelCount >> 8;

/*
    sprintf(text,
              "        Frame: %-8u ( Luma Y / ChromaU / ChromaV )\n"
              "      Average:      ( %7.2f / %7.2f / %7.2f )\n"
              "      Minimum:      (   %3d   /   %3d   /   %3d    )\n"
              "      Maximum:      (   %3d   /   %3d   /   %3d    )\n"
              "Loose Minimum:      (   %3d   /   %3d   /   %3d    )\n"
              "Loose Maximum:      (   %3d   /   %3d   /   %3d    )\n"
  , static_cast<double>(y.GetWeightedTotal()) / yPixelCount
  , static_cast<double>(u.GetWeightedTotal()) / uPixelCount
  , static_cast<double>(v.GetWeightedTotal()) / vPixelCount
  , y.GetMin(), u.GetMin(), v.GetMin()
  , y.GetMax(), u.GetMax(), v.GetMax()
  , y.GetLooseMin(yLooseLimit), u.GetLooseMin(uLooseLimit),  v.GetLooseMin(vLooseLimit)
  , y.GetLooseMax(yLooseLimit), u.GetLooseMax(uLooseLimit),  v.GetLooseMax(vLooseLimit)

  ApplyMessage(&src, vi, text, vi.width/4, 0xa0a0a0,0,0 , env );

*/
  return source;
}



} } } //namespace avs::filters::coloryuv
