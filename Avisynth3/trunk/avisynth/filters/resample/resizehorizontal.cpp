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


//avisynth include
#include "resizehorizontal.h"
#include "../../videoframe.h"
#include "../../videoframe/bufferwindow.h"
#include "../../runtime_environment.h"



namespace avs { namespace filters {


Resize::Horizontal::Horizontal(PClip child, PFilter filter, int width, const SubRange& subrange)
  : Resize( child, filter, subrange )
  , width_( width )
{
  if ( width < 8 )
    ; //appropriate exception ("Resize: Width must be bigger than or equal to 8.");

  GetVideoInfo().SetWidth( width );  //exception if invalid 
}



///////////////////////////////////////////////////////////////////////////////////
//  Resize::Horizontal::RGB24::MakeFrame
//
CPVideoFrame Resize::Horizontal::RGB24::MakeFrame(int n) const
{

  const VideoInfo& vi = GetVideoInfo();

  CPVideoFrame src_frame = GetChildFrame(n);
  PVideoFrame dst_frame = src_frame->BlankClone(vi.GetDimension());

  CWindowPtr src = (*src_frame)[NOT_PLANAR].GetReadPtr();
  WindowPtr dst = (*dst_frame)[NOT_PLANAR].GetWritePtr();
    // RGB24 is not recommended. 75% of all pixels are not aligned.
    
  int y = vi.GetHeight();
  int w = vi.GetWidth() * 3;
    
  int filter_size = pattern.count();  
  int * pattern_ptr = pattern.get() - filter_size;
    
  static const __int64 xFF000000 = 0xFF000000;

  __asm
  {
    mov         esi, src.ptr
    mov         edi, dst.ptr
    pxor        mm2, mm2
    movq        mm4, xFF000000
    align 16
  yloop24:
    xor         ecx, ecx
    mov         edx, pattern_ptr          ;cur - filter_size
    align 16
  xloop24:
    mov         eax, filter_size
    lea         edx, [edx+eax*4]          ;cur += filter_size
    mov         ebx, [edx]
    lea         ebx, [ebx+ebx*2]          ;ebx = ofs = *cur * 3
    add         edx, 4                    ;cur++
    pxor        mm0, mm0                  ;btotal, gtotal
    pxor        mm1, mm1                  ;rtotal
    lea         edx, [edx+eax*4]          ;cur += filter_size
    add         ebx, esi                  ;ebx = src.ptr + ofs*3
    lea         eax, [eax+eax*2]          ;eax = a = filter_size * 3
    align 16
  aloop24:
    sub         edx, 4                    ;cur--
    sub         eax, 3
    movd        mm7, [ebx+eax]            ;mm7 = src.ptr[ofs+a] = 0|0|0|0|x|r|g|b
    punpcklbw   mm7, mm2                  ;mm7 = 0x|0r|0g|0b
    movq        mm6, mm7
    punpcklwd   mm7, mm2                  ;mm7 = 00|0g|00|0b
    punpckhwd   mm6, mm2                  ;mm6 = 00|0x|00|0r
    movd        mm5, [edx]                ;mm5 =    00|co (co = coefficient)
    packssdw    mm5, mm2
    punpckldq   mm5, mm5                  ;mm5 =    co|co
    pmaddwd     mm7, mm5                  ;mm7 =  g*co|b*co
    pmaddwd     mm6, mm5                  ;mm6 =  x*co|r*co
    paddd       mm0, mm7
    paddd       mm1, mm6
    jnz         aloop24
    pslld       mm0, 2
    pslld       mm1, 2                    ;compensate the fact that FPScale = 16384
    packuswb    mm0, mm1                  ;mm0 = x|_|r|_|g|_|b|_
    psrlw       mm0, 8                    ;mm0 = 0|x|0|r|0|g|0|b
    packuswb    mm0, mm2                  ;mm0 = 0|0|0|0|x|r|g|b
    pslld       mm0, 8
    psrld       mm0, 8                    ;mm0 = 0|0|0|0|0|r|g|b
    movd        mm3, [edi+ecx]            ;mm3 = 0|0|0|0|x|r|g|b (dst)
    pand        mm3, mm4                  ;mm3 = 0|0|0|0|x|0|0|0 (dst)
    por         mm3, mm0
    movd        [edi+ecx], mm3

    add         ecx, 3
    cmp         ecx, w
    jnz         xloop24

    add         esi, src.pitch
    add         edi, dst.pitch
    dec         y
    jnz         yloop24
    emms
  }

  return dst_frame;

}


////////////////////////////////////////////////////////////////////////////////
//  Resize::Horizontal::RGB32::MakeFrame
//
CPVideoFrame Resize::Horizontal::RGB32::MakeFrame(int n) const
{

  const VideoInfo& vi = GetVideoInfo();

  CPVideoFrame src_frame = GetChildFrame(n);
  PVideoFrame dst_frame = src_frame->BlankClone(vi.GetDimension());

  CWindowPtr src = (*src_frame)[NOT_PLANAR].GetReadPtr();
  WindowPtr dst = (*dst_frame)[NOT_PLANAR].GetWritePtr();
    
  int y = vi.GetHeight();
  int w = vi.GetWidth();
    
  int filter_size = pattern.count();  
  int * pattern_ptr = pattern.get() - filter_size;

  __asm
  {
    mov         esi, src.ptr
    mov         edi, dst.ptr
    pxor        mm2, mm2
    align 16
  yloop32:
    xor         ecx, ecx
    mov         edx, pattern_ptr          ;cur - filter_size
    align 16
  xloop32:
    mov         eax, filter_size
    lea         edx, [edx+eax*4]          ;cur += filter_size
    mov         ebx, [edx]
    shl         ebx, 2                    ;ebx = ofs = *cur * 4
    add         edx, 4                    ;cur++
    pxor        mm0, mm0                  ;btotal, gtotal
    pxor        mm1, mm1                  ;atotal, rtotal
    lea         edx, [edx+eax*4]          ;cur += filter_size
    add         ebx, esi                  ;ebx = src.ptr + ofs*4
    align 16
  aloop32:
    sub         edx, 4                    ;cur--
    dec         eax
    movd        mm7, [ebx+eax*4]          ;mm7 = srcp[ofs+a] = 0|0|0|0|a|r|g|b
    punpcklbw   mm7, mm2                  ;mm7 = 0a|0r|0g|0b
    movq        mm6, mm7
    punpcklwd   mm7, mm2                  ;mm7 = 00|0g|00|0b
    punpckhwd   mm6, mm2                  ;mm6 = 00|0a|00|0r
    movd        mm5, [edx]                ;mm5 =    00|co (co = coefficient)
    packssdw    mm5, mm2
    punpckldq   mm5, mm5                  ;mm5 =    co|co
    pmaddwd     mm7, mm5                  ;mm7 =  g*co|b*co
    pmaddwd     mm6, mm5                  ;mm6 =  a*co|r*co
    paddd       mm0, mm7
    paddd       mm1, mm6
    jnz         aloop32
    pslld       mm0, 2
    pslld       mm1, 2                    ;compensate the fact that FPScale = 16384
    packuswb    mm0, mm1                  ;mm0 = a|_|r|_|g|_|b|_
    psrlw       mm0, 8                    ;mm0 = 0|a|0|r|0|g|0|b
    packuswb    mm0, mm2                  ;mm0 = 0|0|0|0|a|r|g|b
    movd        [edi+ecx*4], mm0

    inc         ecx
    cmp         ecx, w
    jnz         xloop32

    add         esi, src.pitch
    add         edi, dst.pitch
    dec         y
    jnz         yloop32
    emms
  }

  return dst_frame;
}


CPVideoFrame Resize::Horizontal::YV12::MakeFrame(int n) const
{

  CPVideoFrame src = GetChildFrame(n);
  PVideoFrame dst = src->BlankClone( GetVideoInfo().GetDimension() );

  ProcessPlane( (*src)[PLANAR_Y], (*dst)[PLANAR_Y], luma );
  ProcessPlane( (*src)[PLANAR_U], (*dst)[PLANAR_U], chroma );
  ProcessPlane( (*src)[PLANAR_U], (*dst)[PLANAR_U], chroma );

  return dst;
}



void Resize::Horizontal::ProcessPlane(const BufferWindow& src_, BufferWindow& dst_, const ResamplingPattern::MMXInterleave& pattern)
{

  PEnvironment env = src_.GetEnvironment();

  CWindowPtr src = src_.GetReadPtr();
  WindowPtr dst = dst_.GetWritePtr();

  BYTE * buffer = env->TempBuffer( src_.GetWidth() * 2 + 32, 64 );   // aligned for Athlon cache line

  int * pattern_ptr = pattern.get();
  int filter_size = pattern.count();
  int filter_offset = filter_size * 8 + 8;

  int org_width = src_.GetWidth();
      

  static const __int64 x0000000000FF00FF = 0x0000000000FF00FF;  
  static const __int64 xFFFF0000FFFF0000 = 0xFFFF0000FFFF0000;  
  static const __int64 FPround =           0x0000200000002000;  // 16384/2

  __asm
  {
    pxor        mm0, mm0
    movq        mm7, x0000000000FF00FF
    movq        mm6, FPround
    movq        mm5, xFFFF0000FFFF0000
  }
  
  if ( env->IsISSEEnabled() )
  {
    for ( int y = dst_.GetHeight(); y-- > 0; src.to(0, 1), dst.to(0, 1) )
    {     
      int x = (dst_.GetWidth() + 3) >> 2;

      BYTE * ptr_ = dst.ptr;
      int loopc;
        
      __asm
      {  
        mov         ecx, org_width
        mov         edx, buffer
        mov         esi, src.ptr
        mov         eax, -1
      // deinterleave current line to 00yy 00yy 00yy 00yy
        align 16

      yv_deintloop:
        prefetchnta [esi+256]
        movd        mm1, [esi]          ;mm1 = 00 00 YY YY
        inc         eax
        punpcklbw   mm1, mm0            ;mm1 = 0Y 0Y 0Y 0Y
        movq        [edx+eax*8], mm1    
        add         esi, 4
        sub         ecx, 4

        jnbe        yv_deintloop        //changed to jump if > 0 
      // use this as source from now on
        mov         eax, pattern_ptr
        align 16
      yv_xloopYUV:
        mov         ebx, [filter_offset]  ; Offset to next pixel pair
        mov         ecx, filter_size
        mov         esi, [eax]          ;esi=&tempY[ofs0]
        movq        mm1, mm0            ;Clear mm0
        movq        mm3, mm0            ;Clear mm3
        mov         edi, [eax+4]        ;edi=&tempY[ofs1]
        mov         loopc,ecx
        mov         edx, [eax+ebx]      ;edx = next &tempY[ofs0]
        mov         ecx, [eax+ebx+4]    ;ecx = next &tempY[ofs1]
        add         esi, buffer
        add         edi, buffer
        add         edx, buffer
        add         ecx, buffer
        add         eax, 8              ;pattern_ptr++
        align 16
      yv_aloopY:
        // Identifiers:
        // Ya, Yb: Y values in src.ptr[ofs0]
        // Ym, Yn: Y values in src.ptr[ofs1]
        // Equivalent pixels of next two pixels are in mm4
        movd        mm2, [esi]          ;mm2 =  0| 0|Yb|Ya
         movd        mm4, [edx]
        punpckldq   mm2, [edi]          ;mm2 = Yn|Ym|Yb|Ya
         add         esi, 4
         add         edx, 4
        pmaddwd     mm2, [eax]          ;mm2 = Y1|Y0 (DWORDs)
         punpckldq   mm4, [ecx]         ;[eax] = COn|COm|COb|COa
         add         edi, 4
         add         ecx, 4
        pmaddwd     mm4, [eax+ebx]      ;mm4 = Y1|Y0 (DWORDs)
         add         eax, 8              ;pattern_ptr++
         dec         loopc
        paddd       mm1, mm2            ;accumulate
         paddd       mm3, mm4            ;accumulate

        jz         out_yv_aloopY
//unroll1
        movd        mm2, [esi]          ;mm2 =  0| 0|Yb|Ya
         movd        mm4, [edx]
        punpckldq   mm2, [edi]          ;mm2 = Yn|Ym|Yb|Ya
         add         esi, 4
         add         edx, 4
        pmaddwd     mm2, [eax]          ;mm2 = Y1|Y0 (DWORDs)
         punpckldq   mm4, [ecx]         ;[eax] = COn|COm|COb|COa
         add         edi, 4
         add         ecx, 4
        pmaddwd     mm4, [eax+ebx]      ;mm4 = Y1|Y0 (DWORDs)
         add         eax, 8              ;pattern_ptr++
         dec         loopc
        paddd       mm1, mm2            ;accumulate
         paddd       mm3, mm4            ;accumulate

        jz         out_yv_aloopY
//unroll2
        movd        mm2, [esi]          ;mm2 =  0| 0|Yb|Ya
         movd        mm4, [edx]
        punpckldq   mm2, [edi]          ;mm2 = Yn|Ym|Yb|Ya
         add         esi, 4
         add         edx, 4
        pmaddwd     mm2, [eax]          ;mm2 = Y1|Y0 (DWORDs)
         punpckldq   mm4, [ecx]         ;[eax] = COn|COm|COb|COa
         add         edi, 4
         add         ecx, 4
        pmaddwd     mm4, [eax+ebx]      ;mm4 = Y1|Y0 (DWORDs)
         add         eax, 8              ;pattern_ptr++
         dec         loopc
        paddd       mm1, mm2            ;accumulate
         paddd       mm3, mm4            ;accumulate

        jz         out_yv_aloopY
//unroll3
        movd        mm2, [esi]          ;mm2 =  0| 0|Yb|Ya
         movd        mm4, [edx]
        punpckldq   mm2, [edi]          ;mm2 = Yn|Ym|Yb|Ya
         add         esi, 4
         add         edx, 4
        pmaddwd     mm2, [eax]          ;mm2 = Y1|Y0 (DWORDs)
         punpckldq   mm4, [ecx]         ;[eax] = COn|COm|COb|COa
         add         edi, 4
         add         ecx, 4
        pmaddwd     mm4, [eax+ebx]      ;mm4 = Y1|Y0 (DWORDs)
         add         eax, 8              ;pattern_ptr++
         dec         loopc
        paddd       mm1, mm2            ;accumulate
         paddd       mm3, mm4            ;accumulate

        jz         out_yv_aloopY
//unroll4
        movd        mm2, [esi]          ;mm2 =  0| 0|Yb|Ya
         movd        mm4, [edx]
        punpckldq   mm2, [edi]          ;mm2 = Yn|Ym|Yb|Ya
         add         esi, 4
         add         edx, 4
        pmaddwd     mm2, [eax]          ;mm2 = Y1|Y0 (DWORDs)
         punpckldq   mm4, [ecx]         ;[eax] = COn|COm|COb|COa
         add         edi, 4
         add         ecx, 4
        pmaddwd     mm4, [eax+ebx]      ;mm4 = Y1|Y0 (DWORDs)
         add         eax, 8              ;pattern_ptr++
         dec         loopc
        paddd       mm1, mm2            ;accumulate
         paddd       mm3, mm4            ;accumulate

        jz         out_yv_aloopY
//unroll5
        movd        mm2, [esi]          ;mm2 =  0| 0|Yb|Ya
         movd        mm4, [edx]
        punpckldq   mm2, [edi]          ;mm2 = Yn|Ym|Yb|Ya
         add         esi, 4
         add         edx, 4
        pmaddwd     mm2, [eax]          ;mm2 = Y1|Y0 (DWORDs)
         punpckldq   mm4, [ecx]         ;[eax] = COn|COm|COb|COa
         add         edi, 4
         add         ecx, 4
        pmaddwd     mm4, [eax+ebx]      ;mm4 = Y1|Y0 (DWORDs)
         add         eax, 8              ;pattern_ptr++
         dec         loopc
        paddd       mm1, mm2            ;accumulate
         paddd       mm3, mm4            ;accumulate

        jz         out_yv_aloopY
//unroll6
        movd        mm2, [esi]          ;mm2 =  0| 0|Yb|Ya
         movd        mm4, [edx]
        punpckldq   mm2, [edi]          ;mm2 = Yn|Ym|Yb|Ya
         add         esi, 4
         add         edx, 4
        pmaddwd     mm2, [eax]          ;mm2 = Y1|Y0 (DWORDs)
         punpckldq   mm4, [ecx]         ;[eax] = COn|COm|COb|COa
         add         edi, 4
         add         ecx, 4
        pmaddwd     mm4, [eax+ebx]      ;mm4 = Y1|Y0 (DWORDs)
         add         eax, 8              ;pattern_ptr++
         dec         loopc
        paddd       mm1, mm2            ;accumulate
         paddd       mm3, mm4            ;accumulate

        jz         out_yv_aloopY
//unroll7
        movd        mm2, [esi]          ;mm2 =  0| 0|Yb|Ya
         movd        mm4, [edx]
        punpckldq   mm2, [edi]          ;mm2 = Yn|Ym|Yb|Ya
         add         esi, 4
         add         edx, 4
        pmaddwd     mm2, [eax]          ;mm2 = Y1|Y0 (DWORDs)
         punpckldq   mm4, [ecx]         ;[eax] = COn|COm|COb|COa
         add         edi, 4
         add         ecx, 4
        pmaddwd     mm4, [eax+ebx]      ;mm4 = Y1|Y0 (DWORDs)
         add         eax, 8              ;pattern_ptr++
         dec         loopc
        paddd       mm1, mm2            ;accumulate
         paddd       mm3, mm4            ;accumulate

        jnz         yv_aloopY
        align 16
out_yv_aloopY:

        add eax,ebx;
        mov         edx,ptr_
        paddd       mm1, mm6            ;Y1|Y1|Y0|Y0  (round)
        paddd       mm3, mm6            ;Y1|Y1|Y0|Y0  (round)
        psrld       mm1, 14             ;mm1 = 0|y1|0|y0
        psrld       mm3, 14             ;mm1 = 0|y1|0|y0
        pshufw mm1,mm1,11111000b        ;mm1 = 0|0|y1|y0
        pshufw mm3,mm3,10001111b        ;mm1 = y1|y0|0|0

        packuswb    mm1, mm1            ;mm1 = ...|0|0|0|Y1y0
        packuswb    mm3, mm3            ;mm3 = ...|0|0|Y1y0|0
        por mm1,mm3
        movd        [edx], mm1
        add         ptr_,4
        dec         x
        jnz         yv_xloopYUV        
      }// end asm

    }//for( int y = ...

  } else {  // end if isse, else do mmx
      
    for ( int y = dst_.GetHeight(); y-- > 0; )
    {
      int x = (dst_.GetWidth() + 3) >> 2;

      BYTE * ptr_ = dst.ptr;
      int loopc;
    
      __asm
      {  
        mov         ecx, org_width
        mov         edx, buffer;
        mov         esi, src.ptr
        mov         eax, -1
      // deinterleave current line to 00yy 00yy 00yy 00yy
        align 16

      yv_deintloop_mmx:
        movd        mm1, [esi]          ;mm1 = 00 00 YY YY
        inc         eax
        punpcklbw   mm1, mm0            ;mm1 = 0Y 0Y 0Y 0Y
        movq        [edx+eax*8], mm1    
        add         esi, 4
        sub         ecx, 4

        jnbe        yv_deintloop_mmx    //changed to jnbe
      // use this as source from now on
        mov         eax, pattern_ptr
        align 16
      yv_xloopYUV_mmx:
        mov         ebx, [filter_offset]  ; Offset to next pixel pair
        mov         ecx, filter_size
        mov         esi, [eax]          ;esi=&tempY[ofs0]
        movq        mm1, mm0            ;Clear mm0
        movq        mm3, mm0            ;Clear mm3
        mov         edi, [eax+4]        ;edi=&tempY[ofs1]
        mov         loopc,ecx
        mov         edx, [eax+ebx]      ;edx = next &tempY[ofs0]
        mov         ecx, [eax+ebx+4]    ;ecx = next &tempY[ofs1]
        add         esi, buffer
        add         edi, buffer
        add         edx, buffer
        add         ecx, buffer
        add         eax, 8              ;pattern_ptr++
        align 16
      yv_aloopY_mmx:
        // Identifiers:
        // Ya, Yb: Y values in srcp[ofs0]
        // Ym, Yn: Y values in srcp[ofs1]
        // Equivalent pixels of next two pixels are in mm4
        movd        mm2, [esi]          ;mm2 =  0| 0|Yb|Ya
         movd        mm4, [edx]
        punpckldq   mm2, [edi]          ;mm2 = Yn|Ym|Yb|Ya
         add         esi, 4
         add         edx, 4
        pmaddwd     mm2, [eax]          ;mm2 = Y1|Y0 (DWORDs)
         punpckldq   mm4, [ecx]         ;[eax] = COn|COm|COb|COa
         add         edi, 4
         add         ecx, 4
        pmaddwd     mm4, [eax+ebx]      ;mm4 = Y1|Y0 (DWORDs)
         add         eax, 8              ;pattern_ptr++
         dec         loopc
        paddd       mm1, mm2            ;accumulate
         paddd       mm3, mm4            ;accumulate
        jnz         yv_aloopY_mmx

        // we don't bother unrolling, since non-isse machines have small branch-misprediction penalties.
        add eax,ebx;
        mov         edx,ptr_
        paddd       mm1, mm6            ;Y1|Y1|Y0|Y0  (round)
        paddd       mm3, mm6            ;Y1|Y1|Y0|Y0  (round)
        psrld       mm1, 14             ;mm1 = 0|y1|0|y0
        psrld       mm3, 14             ;mm1 = 0|y1|0|y0
        packuswb    mm1, mm0            ;mm1 = ...|0|0|y1|y0 
        packuswb    mm3, mm0            ;mm3 = ...|0|0|y1|y0
        packuswb    mm1, mm0            ;mm1 = ...|0|0|0|Y1y0
        packuswb    mm3, mm0            ;mm3 = ...|0|0|0|Y1y0
        psllq       mm3,16              ;mm3= 0|0|y1y0|0
        por mm1,mm3  

        movd        [edx], mm1
        add         ptr_,4
        dec         x
        jnz         yv_xloopYUV_mmx
      }// end asm

    } // end for y

  } // end mmx

  __asm { emms }

}



}; }; //namespace avs::filters
