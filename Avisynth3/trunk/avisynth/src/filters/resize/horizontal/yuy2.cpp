// Avisynth v3.0 alpha.  Copyright 2004 David Pierre - Ben Rudiak-Gould et al.
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
#include "../../../core/videoframe.h"
#include "../../../core/runtime_environment.h"


namespace avs { namespace filters { namespace resize { namespace horizontal {


//asm equivalent code
extern "C" void resize_horizontal_yuy2_mmx_nasm(BYTE const* src_ptr, BYTE * dst_ptr, int src_width,
    BYTE * tempY, BYTE * tempUV, int const* pptrUV, int const* pptrY, int countY, int countUV, int x);

extern "C" void emms_();



void YUY2::ResizeFrame(VideoFrame const& source, VideoFrame& target) const
{

  CWindowPtr src = source.ReadFrom(NOT_PLANAR);
  WindowPtr dst = target.WriteTo(NOT_PLANAR);

  src.width = (src.width + 4) / 8;        //we work on 4 pixels at a time (8bytes)
  dst.width = (dst.width + 4) / 8;        //idem

  //creating temp buffers using env (both 8 bytes aligned)
  Block block = GetEnvironment()->TempBlock( src.width * 8 * 2 + 16 );
  BYTE * tempY = block.get();
  BYTE * tempUV = tempY + src.width * 8 + 8;

  int const * pptrY  = luma_.get();
  int const * pptrUV = chroma_.get();

  int countY  = luma_.count() >> 2;       //we use 4 coeffs per  Y loop
  int countUV = chroma_.count() >> 1;     //we use 2 coeffs per UV loop 

  
  for( int y = dst.height; y-- > 0; src.to(0, 1), dst.to(0, 1) )
  {

#if defined(_INTEL_ASM) && ! defined(_FORCE_NASM)

    int x = dst.width;
    BYTE * dstp = dst.ptr;

    __asm
    {

      xor         eax, eax                //eax = 0
      mov         ebx, src.ptr
      mov         ecx, src.width         
      mov         esi, tempY
      mov         edi, tempUV

      pcmpeqw     mm7, mm7                //mm7 = FFFFFFFFFFFFFFFF
      psrlw       mm7, 8                  //mm7 = 00FF00FF00FF00FF   Y mask

      align 16

    deintloop:

      movq        mm2, [ebx + 8*eax]      //mm2 = VYUY|VYUY
      pxor        mm4, mm4                //mm4 = 0
      movq        mm3, mm2                //mm3 = VYUY|VYUY
      pand        mm2, mm7                //mm2 = 0Y0Y0Y0Y
      psrlw       mm3, 8                  //mm5 = Vb|Ub|Va|Ua
      movq        [esi + 8*eax], mm2      //writes to tempY      
      punpckhwd   mm4, mm3                //mm4 = Vb| 0|Ub| 0
      punpcklwd   mm3, mm0                //mm3 =  0|Va| 0|Ua
      por         mm3, mm4                //mm3 = Vb|Va|Ub|Ua
      movq        [edi + 8*eax], mm3      //writes to tempUV
  
      inc         eax                     //++eax
      dec         ecx                     //--ecx
      jnz         deintloop

      mov         edx, pptrUV             //edx points in chroma pattern
      mov         edi, pptrY              //edi points in luma pattern

      align 16

    xloop:

      push        countY                  //aloop counter on stack
      push        edx                     //save edx (chroma pattern) on stack

      mov         eax, [edi]              //eax = offset0
      mov         ebx, [edi + 4]          //ebx = offset1
      mov         ecx, [edi + 8]          //ecx = offset2
      mov         edx, [edi + 12]         //edx = offset3
      mov         esi, tempY
      add         edi, 16                 //edx += 16 now points to the coeffs

      pcmpeqd     mm0, mm0                //mm0 = FFFFFFFFFFFFFFFF
      psrlq       mm0, 51                 //mm0 = 0000000000001FFF    init rounded
      movq        mm1, mm0                //mm1 = 0000000000001FFF    init rounded
      movq        mm2, mm0                //mm2 = 0000000000001FFF    init rounded
      movq        mm3, mm0                //mm3 = 0000000000001FFF    init rounded

      align 16

    aloopY:

      movq        mm4, [esi + 2*eax]      //mm4 = Yd|Yc|Yb|Ya
      movq        mm5, [esi + 2*ebx]      //mm5 = Yi|Yh|Yg|Yf
      movq        mm6, [esi + 2*ecx]      //mm6 = Yp|Yo|Yn|Ym
      movq        mm7, [esi + 2*edx]      //mm7 = Yv|Yu|Yt|Ys
      pmaddwd     mm4, [edi]              //mm4 =  Y0b | Y0a    [edi]    = Cd|Cc|Cb|Ca
      pmaddwd     mm5, [edi + 8]          //mm5 =  Y1b | Y1a  [edi + 8]  = Ci|Ch|Cg|Cf
      pmaddwd     mm6, [edi + 16]         //mm6 =  Y2b | Y2a  [edi + 16] = Cp|Co|Cn|Cm
      pmaddwd     mm7, [edi + 24]         //mm7 =  Y3b | Y3a  [edx + 24] = Cv|Cu|Ct|Cs
      paddd       mm0, mm4                //accumulate Y0
      paddd       mm1, mm5                //accumulate Y1
      paddd       mm2, mm4                //accumulate Y2
      paddd       mm3, mm5                //accumulate Y3

      add         edi, 32                 //edx += 32  next coeffs
      add         esi, 8                  //esi += 8   next src Y

      dec         [esp + 4]               //--aloop counter
      jnz         aloopY

      pop         edx                     //restore edx (chroma pattern) from stack 
      pop         eax                     //get rid of obsolete aloopY counter

      mov         eax, [edx]              //eax = offset0
      mov         ebx, [edx + 4]          //ebx = offset1
      mov         ecx, countUV
      add         edx, 8                  //edx += 8  now points to coeffs
      mov         esi, tempUV

      pcmpeqd     mm4, mm4                //mm4 = FFFFFFFFFFFFFFFF
      psrld       mm4, 19                 //mm4 = 00001FFF00001FFF   init rounded
      movq        mm5, mm4                //mm5 = 00001FFF00001FFF   init rounded

      align 16

    aloopUV:

      movq        mm6, [edx]              //mm6 = Cn|Cm|Cb|Ca
      movq        mm7, mm6                //mm7 = Cn|Cm|Cb|Ca
      punpcklwd   mm6, mm6                //mm6 = Cb|Ca|Cb|Ca
      punpcklwd   mm7, mm7                //mm7 = Cn|Cm|Cn|Cm
      pmaddwd     mm6, [esi + 4*eax]      //mm6 =   V0 | U0   [esi + 4*eax] = Vb|Va|Ub|Ua
      pmaddwd     mm7, [esi + 4*ebx]      //mm7 =   V1 | U1   [esi + 4*ebx] = Vn|Vm|Un|Um
      paddd       mm4, mm6                //accumulate U0 V0
      paddd       mm5, mm7                //accumulate U1 V1

      add         edx, 8                  //edi += 8  next coeffs
      add         esi, 8                  //esi += 8  next src UV

      dec         ecx                     //--ecx
      jnz         aloopUV


      movq        mm6, mm0                //mm6 =  Y0b | Y0a
      pslld       mm4, 2                  //mm4 =  V0--|U0--   compensate 14 bits fractional
      movq        mm7, mm2                //mm7 =  Y2b | Y2a
      pslld       mm5, 2                  //mm5 =  V1--|U1--   compensate 14 bits fractional
      punpckhwd   mm6, mm1                //mm6 =  Y1b | Y0b
      packuswb    mm4, mm5                //mm4 =  v-u-|v-u-   (- is garbage)
      punpckhwd   mm7, mm3                //mm7 =  Y3b | Y2b
      psrlw       mm4, 8                  //mm4 =  0v0u|0v0u
      punpcklwd   mm0, mm1                //mm0 =  Y1a | Y0a
      psllw       mm4, 8                  //mm4 =  v0u0|v0u0
      punpcklwd   mm2, mm3                //mm2 =  Y3a | Y2a
      paddd       mm0, mm6                //mm0 =  Y1  | Y0
      paddd       mm2, mm7                //mm2 =  Y3  | Y2

      pslld       mm0, 2                  //mm0 =  Y1--|Y0--   compensate 14 bits fractional
      pslld       mm2, 2                  //mm2 =  Y3--|Y2--   compensate 14 bits fractional
      packuswb    mm0, mm2                //mm0 =  y-y-|y-y-
      psrlw       mm0, 8                  //mm0 =  0y0y|0y0y  
      por         mm0, mm4                //mm0 =  vyuy|vyuy

      mov         ebx, dstp               //ebx points to dst
      add         dstp, 8                 //dstp +=8   next pixel couple
      movq        [ebx], mm0              //write result

      dec         x                       //--x
      jnz         xloop
    }

#else 

  //use nasm code
  resize_horizontal_yuy2_mmx_nasm(src.ptr, dst.ptr, src.width, tempY, tempUV,
			     pptrUV, pptrY, countY, countUV, dst.width);

#endif // defined(_INTEL_ASM) && ! defined(_FORCE_NASM)

  }//for


#ifdef _INTEL_ASM

  __asm { emms }

#else
  #ifdef _ATT_ASM

  __asm __volatile ("emms;":::"memory");

  #else

  emms_();

  #endif //_ATT_ASM
#endif

}



} } } }  //namespace avs::filters::resize::horizontal
