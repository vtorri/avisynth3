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


//avisynth include
#include "rgb24.h"
#include "../../../core/videoframe.h"


namespace avs { namespace filters { namespace resize { namespace horizontal {



void RGB24::ResizeFrame(VideoFrame const& source, VideoFrame& target) const
{

  CWindowPtr src = source.ReadFrom(NOT_PLANAR);
  WindowPtr dst = target.WriteTo(NOT_PLANAR);
  
 
  int count = GetPattern().count();       //coeff count
  int const * pptr = GetPattern().get();  //pattern ptr
    
  int y = dst.height;                     //y loop counter
  int pad = dst.padValue();               //padding from end of dst row to start of next one


#ifdef _INTEL_ASM

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
    lea         ebx, [ebx + 2*ebx]        //ebx *= 3  byte offset to the ref pixel
    add         edx, 4                    //edx += 4  now points to the coeffs 

    movq        mm0, mm6                  //mm0 = mm6  ( G total | B total )  init rounded
    movq        mm1, mm6                  //mm1 = mm6  ( X total | R total )  init rounded

    align 16

  aloop:

    movd        mm2, [esi + ebx]          //mm2 = 0|0|0|0|x|r|g|b  read src pixel
    punpcklbw   mm2, mm7                  //mm2 = 0x|0r|0g|0b
    movq        mm3, mm2                  //mm3 = 0x|0r|0g|0b
    punpcklwd   mm2, mm7                  //mm2 = 00|0g|00|0b
    punpckhwd   mm3, mm7                  //mm3 = 00|0x|00|0r
    movd        mm5, [edx]                //mm5 = 00|00|coeff 
    packssdw    mm5, mm5                  //mm5 = 00|co|00|co  coeff saturated (useless if always positive: to check)
    pmaddwd     mm2, mm5                  //mm2 =  g*co|b*co
    add         edx, 4                    //edx += 4  next coeff
    pmaddwd     mm3, mm5                  //mm3 =  x*co|r*co
    add         ebx, 3                    //ebx += 3  next src pixel
    dec         eax                       //--eax
    paddd       mm0, mm2                  //accumulates g and b
    paddd       mm1, mm3                  //accumulates r (and x)

    jnz         aloop                     //jump if eax != 0

    psrld       mm0, 14                   //mm0 = 00|0g|00|0b    kills 14 bit fraction
    psrld       mm1, 14                   //mm1 = 00|0x|00|0r    kills 14 bit fraction
    packuswb    mm0, mm1                  //mm0 = 0|x|0|r|0|g|0|b
    packuswb    mm0, mm2                  //mm0 = 0|0|0|0|x|r|g|b
    movd        [edi], mm0                //write an extra x in memory, but we don't care
                                          //next write will overwrite it by a significant value
    add         edi, 3
    sub         ecx, 3
    jnz         xloop

    add         esi, src.pitch            //move to next src
    add         edi, pad                  //move to next dst line

    dec         y                         //--y
    jnz         yloop

    emms
  }

#else
#error "resize horizontal RGB24: missing code path"
#endif //_INTEL_ASM


}



} } } } //namespace avs::filters::resize::horizontal
