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
#include "spatialsoften.h"
#include "../../videoinfo.h"


namespace avs { namespace filters {



CPVideoFrame  SpatialSoften::MakeFrame(int n) const
{
  CPVideoFrame src_frame = GetChildFrame(n);
  PVideoFrame  dst_frame = VideoFrame::Create(GetVideoInfo(), GetEnvironment());

  Dimension dim = src_frame->GetDimension();
  CWindowPtr src = (*src_frame)[NOT_PLANAR].GetReadPtr();
  WindowPtr dst = (*dst_frame)[NOT_PLANAR].GetWritePtr();
  int row_size = dim.GetWidth()<<1;

  void * buffer = GetEnvironment()->GetTempBuffer( (dim.GetHeight() + diameter - 1) * sizeof(BYTE *), sizeof(BYTE *) );
  const BYTE ** line = (const BYTE **) buffer;

  for(int h = diameter>>1; h-- > 0; ++line)
    *line = src.ptr;
  for(int h = 0; h < dim.GetHeight(); ++h, ++line)
    *line = src.to(0, h);
  for(int h = diameter>>1; h-- > 0; ++line)
    *line = src.to(0, dim.GetHeight() - 1);

  line = (const BYTE **) buffer;


  for(int y = 0; y < dim.GetHeight(); ++y, ++line)
  {
    int x = 0;

    int edge = (diameter + 1) & -4;   

    for(; x < edge; ++x)      // diameter-1 == (diameter>>1) * 2
      dst(x, y) = src(x, y);

    for(; x < row_size - edge; x += 2)
    {
      int cnt = 0, _y = 0, _u = 0, _v = 0;
      int xx = x | 3;
      int Y = src(x, y), U = src(xx - 2, y), V = src(xx, y);
  
      for (int h = 0; h < diameter; ++h) 
      {
        for (int w = - diameter + 1; w < diameter; w += 2) 
        {
          int xw = (x+w) | 3;
          if (IsClose(line[h][x+w], Y, luma_threshold) && IsClose(line[h][xw-2], U,
                      chroma_threshold) && IsClose(line[h][xw], V, chroma_threshold)) 
          {
            ++cnt; _y += line[h][x+w]; _u += line[h][xw-2]; _v += line[h][xw];
          }
        }
      }
      
      dst(x, y) = (_y + (cnt>>1)) / cnt;

      if ( ! (x & 3) )
      {
        dst(x + 1, y) = (_u + (cnt>>1)) / cnt;
        dst(x + 3, y) = (_v + (cnt>>1)) / cnt;
      }
    }

    for(; x<row_size; ++x)
      dst(x, y) = src(x, y);
  }

  GetEnvironment()->RetrieveBuffers();

  return dst;
}










}; }; //namespace avs::filters