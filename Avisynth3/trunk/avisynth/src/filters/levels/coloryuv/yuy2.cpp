// Avisynth v3.0 alpha.  Copyright 2004 Ben Rudiak-Gould et al.
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
#include "yuy2.h"
#include "lumamap.h"
#include "chromamap.h"
#include "../../../core/videoframe.h"
#include "../../../core/cow_shared_ptr.h"
#include "../../../utility/bytecounter.h"


namespace avs { namespace filters { namespace coloryuv {



void YUY2::Count(VideoFrame const& frame, ByteCounter& y, ByteCounter& u, ByteCounter& v) const
{
  CWindowPtr wp = frame.ReadFrom(NOT_PLANAR);

  y.Count<2>(wp);            //counts Y values

  wp.to(1, 0);
  y.Count<4>(wp);            //counts U values

  wp.to(2, 0);
  v.Count<4>(wp);            //counts V values
}


void YUY2::ApplyMaps(VideoFrame& frame, LumaMap const& y, ChromaMap const& u, ChromaMap const& v) const
{
  WindowPtr wp = frame.WriteTo(NOT_PLANAR);

  y.ApplyTo<2>(wp);          //looks up Y values 

  wp.to(1, 0);
  u.ApplyTo<4>(wp);          //looks up U values

  wp.to(2, 0);
  v.ApplyTo<4>(wp);          //looks up V values
}



/*
CPVideoFrame YUY2::MakeFrame(CPVideoFrame const& source) const
{
  PVideoFrame frame = source;
  WindowPtr wp = frame->WriteTo(NOT_PLANAR);
  unsigned char *srcp = wp.ptr;

  if (analyze||autowhite||autogain)
  {
    ByteCounter y, u, v;

    Count(*frame, y, u, v);

    long yPixelCount = y.GetTotal();
    long uPixelCount = u.GetTotal();
    long vPixelCount = v.GetTotal();

    float avg_u=0, avg_v=0, avg_y=0;
    int Amin_u=0, Amin_v=0, Amin_y=0;
    int Amax_u=0, Amax_v=0, Amax_y=0;

    int At_y2=int(floorf(pixels/256.0f+0.5f)); // When 1/256th of all pixels have been reached, trigger "Loose min/max"
    int At_uv2=int(floorf(pixels/1024.0f+0.5f)); 
    
    for (i=0;i<256;i++)
    {
      avg_y+=float(yPParam_.accum[i]*i);
      avg_u+=float(uPParam_.accum[i]*i);
      avg_v+=float(vPParam_.accum[i]*i);
      
    }
    
    //Second part specific to YUY2
    float Favg_y=avg_y/(float)pixels;
    //uvdiv = 2.0f
    float Favg_u=(avg_u*2.0f)/(float)pixels;
    float Favg_v=(avg_v*2.0f)/(float)pixels;

    if (autowhite)
    {
      boost::get<1>(uIParam_)=127-(int)Favg_u;
      boost::get<1>(vIParam_)=127-(int)Favg_v;
    }
    if (autogain)
    {
      Amax_y=std::min(Amax_y,236);
      Amin_y=std::max(Amin_y,16);  // Never scale above luma range!
      if (Amin_y!=Amax_y)
      {
        int y_range = Amax_y-Amin_y;
        float scale = (220.0 / y_range);
        boost::get<0>(yIParam_) = (int) (256.0 * scale)-256;
        boost::get<1>(yIParam_) = -(int)(scale * (float)(Amin_y)-16);
      }
    }
    MakeGammaLUT();
  }

  //Third and last part specific to YUY2
  srcp = wp.ptr;
  for (y=0; y<wp.height; y++)
  {
    for (x=0; x<wp.width; x+=4)
    {
      //FIXME: endianess
      srcp[x+0] = yPParam_.LUT[srcp[x+0]];
      srcp[x+1] = uPParam_.LUT[srcp[x+1]];
      srcp[x+2] = yPParam_.LUT[srcp[x+2]];
      srcp[x+3] = vPParam_.LUT[srcp[x+3]];
    }
    srcp += wp.pitch;
  }
  return frame;
}*/

} } } //namespace avs::filters::coloryuv
