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


VideoFrame::YV12::YV12(YUY2 const& source)
  : YUV( ColorSpace::yv12(), source )
{

  WindowPtr y = GetY().GetWritePtr();
  WindowPtr u = GetU().GetWritePtr();
  WindowPtr v = GetV().GetWritePtr();
  CWindowPtr yuy2 = source.GetMain().GetReadPtr();


/* old C++ code (unused)
  int* srcp = (int*)src->GetReadPtr();
  int* dstpY = (int*)dst->GetWritePtr(PLANAR_Y);
  short* dstpV = (short*)dst->GetWritePtr(PLANAR_V);
  short* dstpU = (short*)dst->GetWritePtr(PLANAR_U);
  int xloops=src->GetRowSize()/8;  // One loop = four pixels = two int32
  int yloops=src->GetHeight()/2;   // One loops = 2 lines  (should be 4 for proper chroma interpolation on fieldbased)  
  int src_pitch=src->GetPitch()/4;
  int dst_pitchY=dst->GetPitch(PLANAR_Y)/4;
  int dst_pitchUV=dst->GetPitch(PLANAR_V)/2;
  for (int y=0;y<yloops;y++) {
    for (int x=0;x<xloops;x++) {
      unsigned int s0=srcp[x*2]; // 2 pixels
      unsigned int s1=srcp[x*2+1];
      unsigned int s2=srcp[x*2+src_pitch];
      unsigned int s3=srcp[x*2+src_pitch+1];
      dstpY[x] = (s0&0xff) | ((s0>>8)&0xff00) | ((s1<<16)&0xff0000) | ((s1<<8)&0xff000000);
      dstpY[x+dst_pitchY] = (s2&0xff) | ((s2>>8)&0xff00) | ((s3<<16)&0xff0000) | ((s3<<8)&0xff000000);
      dstpU[x] = (unsigned short)(((s0&0xff00) + (s2&0xff00))>>9) | ((((s1&0xff00)+(s3&0xff00))>>9)<<8);
      dstpV[x] = (unsigned short)(((s0>>24)+(s2>>24))>>1) | ((((s1>>24)+(s3>>24))>>1)<<8);
    }
    srcp+=(src_pitch*2);
    dstpY+=(dst_pitchY*2);
    dstpU+=(dst_pitchUV);
    dstpV+=(dst_pitchUV);
  }
  return dst;
  */


}