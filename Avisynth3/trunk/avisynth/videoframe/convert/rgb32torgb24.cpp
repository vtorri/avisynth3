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
#include "../rgb24videoframe.h"
#include "../rgb32videoframe.h"
#include "../../colorspace/rgb24.h"


namespace avs {



RGB24VideoFrame::RGB24VideoFrame(const RGB32VideoFrame other)
  : InterleavedVideoFarme( CS::RGB24::instance(), other )
{

  CWindowPtr src = other.GetMain().GetReadPtr();
  WindowPtr dst = GetMain().GetWritePtr();

  const Dimension dim = GetDimension();
  const int h = dim.GetHeight();
  const int x_left = dim.GetWidth() % 4;

  int x_loops = 4 * (dim.GetWidth() - x_left);   // 4 dwords/loop   read 16 bytes, write 12 bytes

  __int64 oxooooooooooffffff = 0x0000000000ffffff;
  __int64 oxooffffffoooooooo = 0x00ffffff00000000;

  __asm {
    mov esi,src.ptr
    mov edi,dst.ptr
    mov eax,[h]
    mov edx,[x_left];
    movq mm6,[oxooooooooooffffff];
    movq mm7,[oxooffffffoooooooo];
    align 16
yloop:
    mov ebx,0  ; src offset
    mov ecx,0  ; dst offset
xloop:
    movq mm0,[ebx+esi]    ; a1r1 g1b1 a0r0 g0b0
    movq mm1,[ebx+esi+8]  ; a3r3 g3b3 a2r2 g2b2
    
    movq mm2,mm0      ; a1r1 g1b1 a0r0 g0b0
    movq mm3,mm1      ; a3r3 g3b3 a2r2 g2b2
    
    pand mm0,mm6      ; 0000 0000 00r0 g0b0
    pand mm1,mm6      ; 0000 0000 00r2 g2b2 

    pand mm2,mm7      ; 00r1 g1b1 0000 0000
    pand mm3,mm7      ; 00r3 g3b3 0000 0000

    movq mm4,mm1      ; 0000 0000 00r2 g2b2 
    psrlq mm2,8       ; 0000 r1g1 b100 0000
    
    psllq mm4,48      ; g2b2 0000 0000 0000
    por mm0,mm2       ; 0000 r1g1 b1r0 g0b0

    psrlq mm1,16      ; 0000 0000 0000 00r2
    por mm0,mm4

    psrlq mm3,24      ; 0000 0000 b3g3 r300
    movq [ecx+edi],mm0

    por mm3,mm1
    movd [ecx+edi+8],mm3

    add ebx,16
    add ecx,12
    cmp ebx,[x_loops]
    jl xloop

    
    cmp edx,0
    je no_copy
    cmp edx,2
    je copy_2
    cmp edx,1
    je copy_1
//copy 3
    add esi,ebx
    add edi,ecx
    movsb 
    movsb
    movsb
    inc esi
    movsb
    movsb
    movsb
    inc esi
    movsb
    movsb
    movsb
    sub esi,ebx
    sub edi,ecx
    sub esi,11
    sub edi,9
    jmp no_copy
    align 16
copy_2:
    add esi,ebx
    add edi,ecx
    movsb
    movsb
    movsb
    inc esi
    movsb
    movsb
    movsb
    sub esi,ebx
    sub edi,ecx
    sub esi,7
    sub edi,6
    jmp no_copy
    align 16
copy_1:
    add esi,ebx
    add edi,ecx
    movsb
    movsb
    movsb
    sub esi,ebx
    sub edi,ecx
    sub esi,3
    sub edi,3
    align 16
no_copy:
    add esi,[src.pitch]
    add edi,[dst.pitch]    
    
    dec eax
    jnz yloop
    emms
  }
  
/////////////////////////
//  C++ version
//  
//  for (int y = dim.GetHeight(); y-- > 0; src.to(0, 1), dst.to(0, 1) )
//    for (int x = 0; x < dim.GetWidth(); ++x)
//    {
//      dst.ptr[x*3+0] = src.ptr[x*4+0];
//      dst.ptr[x*3+1] = src.ptr[x*4+1];
//      dst.ptr[x*3+2] = src.ptr[x*4+2];
//    }
//  

}



}; //namespace avs







