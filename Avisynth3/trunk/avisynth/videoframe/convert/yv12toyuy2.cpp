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
#include "../vframe_yuy2.h"
#include "../vframe_yv12.h"
#include "../../colorspace.h"


namespace avs {


VideoFrame::YUY2::YUY2(YV12 const& source)
  : Interleaved( ColorSpace::yuy2(), source )
{

  CWindowPtr y = source.GetY().GetReadPtr();
  CWindowPtr u = source.GetU().GetReadPtr();
  CWindowPtr v = source.GetV().GetReadPtr();
  WindowPtr yuy2 = GetMain().GetWritePtr();


  //calling conversion method from XVID
  //TODO: make own version taking interlacing into account
  yv12_to_yuyv_mmx( yuy2.ptr, yuy2.pitch/2, y.ptr, u.ptr, v.ptr, y.pitch, u.pitch, y.width, y.height );

  /*
    const BYTE* yp = src->GetReadPtr(PLANAR_Y);
    const BYTE* up = src->GetReadPtr(PLANAR_U);
    const BYTE* vp = src->GetReadPtr(PLANAR_V);

    int dst_pitch=dst->GetPitch();
    int src_pitchUV=src->GetPitch(PLANAR_U);
    int src_pitchY=src->GetPitch(PLANAR_Y);

  if (!vi.IsFieldBased()) {
    for (int y=0; y<(vi.height>>1); y++) {
      for (int x=0; x<(vi.width>>1); x++) {
        yuv[x*4]=yp[x*2];
        yuv[x*4+2]=yp[x*2+1];
        yuv[x*4+1]=up[x];
        yuv[x*4+3]=vp[x];

        yuv[x*4+dst_pitch]=yp[x*2+src_pitchY];
        yuv[x*4+2+dst_pitch]=yp[x*2+1+src_pitchY];
        yuv[x*4+1+dst_pitch]=up[x];
        yuv[x*4+3+dst_pitch]=vp[x];
      }
      yp += src_pitchY*2;
      up += src_pitchUV;
      vp += src_pitchUV;
      yuv += dst_pitch*2;
    }
  } else { // Fieldbased
    for (int y=0; y<(vi.height>>2); y++) {
      for (int x=0; x<(vi.width>>1); x++) {
        yuv[x*4]=yp[x*2];
        yuv[x*4+2]=yp[x*2+1];
        yuv[x*4+1]=up[x];
        yuv[x*4+3]=vp[x];

        yuv[x*4+dst_pitch*2]=yp[x*2+src_pitchY*2];  // Two lines down, same UV
        yuv[x*4+2+dst_pitch*2]=yp[x*2+1+src_pitchY*2];
        yuv[x*4+1+dst_pitch*2]=up[x];
        yuv[x*4+3+dst_pitch*2]=vp[x];

        yuv[x*4+dst_pitch]=yp[x*2+src_pitchY];
        yuv[x*4+2+dst_pitch]=yp[x*2+1+src_pitchY];
        yuv[x*4+1+dst_pitch]=up[x+src_pitchUV];
        yuv[x*4+3+dst_pitch]=vp[x+src_pitchUV];

        yuv[x*4+dst_pitch*3]=yp[x*2+src_pitchY*3];
        yuv[x*4+2+dst_pitch*3]=yp[x*2+1+src_pitchY*3];
        yuv[x*4+1+dst_pitch*3]=up[x+src_pitchUV];
        yuv[x*4+3+dst_pitch*3]=vp[x+src_pitchUV];
      }
      yp += src_pitchY*4;
      up += src_pitchUV*2;
      vp += src_pitchUV*2;
      yuv += dst_pitch*4;
    }
  }

    */
}


} //namespace avs