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
#include "resize_horizontal.h"
#include "../../videoframe.h"
#include "../../bufferwindow.h"


namespace avs { namespace filters {



void Resize::Horizontal::RGB45::ResizeFrame(CPVideoFrame source, PVideoFrame target) const
{

  CWindowPtr src = (*source)[NOT_PLANAR].GetReadPtr();
  WindowPtr dst = (*target)[NOT_PLANAR].GetWritePtr();
  
      
  int count = GetPattern().count();       //coeff count
  int const * pptr = GetPattern().get();  //pattern ptr
    
  int y = dst.height;                     //y loop counter
  int pad = dst.pîtch - dst.width;        //padding from end of dst row to start of next one

  __asm
  {
    mov         esi, src.ptr
    mov         edi, dst.ptr

    pcmpeqd     mm6, mm6                  //mm6 = FFFFFFFFFFFFFFFF
    pxor        mm7, mm7                  //mm7 = 0
    psrld       mm6, 19                   //mm6 = 00001FFF00001FFF    13 bits rounder

    align 16
  
  yloop:

    mov         ecx, dst.width            //ecx = dst.width  (x loop counter)
    mov         edx, pptr                 //edx points into the pattern                 

    align 16

  xloop:

    mov         ebx, [edx]                //read from pattern: ebx = ref pixel offset
    mov         eax, count                //eax = count  (a loop counter)
    add         ebx, ebx                  //ebx *= 2
    add         edx, 4                    //edx += 4  now points to the coeffs 
    lea         ebx, [ebx + 2*ebx]        //ebx *= 3  byte offset to the ref pixel

    movq        mm0, mm6                  //mm0 = mm6  ( G total | B total )  init rounded
    movq        mm1, mm6                  //mm1 = mm6  ( X total | R total )  init rounded

    align 16

  aloop:

    movq        mm2, [esi + ebx]          //mm2 = X|R|G|B  read src pixel
    movq        mm3, mm2                  //mm3 = X|R|G|B
    punpcklwd   mm2, mm7                  //mm2 = 0|G|0|B
    punpckhwd   mm3, mm7                  //mm3 = 0|X|0|R
    movd        mm5, [edx]                //mm5 = 0|0|coef 
    packssdw    mm5, mm5                  //mm5 = 0|c|0|c  coeff saturated (useless if always positive: to check)
    pmaddwd     mm2, mm5                  //mm2 = G*c|B*c
    add         edx, 4                    //edx += 4  next coeff
    pmaddwd     mm3, mm5                  //mm3 = X*c|R*c
    add         ebx, 6                    //ebx += 6  next src pixel
    dec         eax                       //--eax
    paddd       mm0, mm2                  //accumulates G and B
    paddd       mm1, mm3                  //accumulates R (and X)

    jnz         aloop                     //jump if eax != 0

    psrld       mm0, 14                   //mm0 = 0|G|0|B    kills 14 bit fraction
    psrld       mm1, 14                   //mm1 = 0|X|0|R    kills 14 bit fraction
    packuswb    mm0, mm1                  //mm0 = X[R[G[B
    movq        [edi], mm0                //write an extra X in memory, but we don't care
                                          //next write will overwrite it by a significant value
    add         edi, 6
    sub         ecx, 6
    jnz         xloop

    add         esi, src.pitch            //move to next src
    add         edi, pad                  //move to next dst line

    dec         y                         //--y
    jnz         yloop

    emms
  }
}



} } //namespace avs::filters