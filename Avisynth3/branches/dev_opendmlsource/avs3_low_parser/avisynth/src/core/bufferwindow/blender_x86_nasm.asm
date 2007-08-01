;; Avisynth v3.0 alpha.  Copyright 2004 David Pierre - Ben Rudiak-Gould et al.
;; http://www.avisynth.org

;; Blends two planes. A weight between the two planes are given.
;; Has rather ok pairing, and has very little memory usage.
;; Processes four pixels per loop, so rowsize must be mod 4.
;; Thanks to ARDA for squeezing out a bit more performance.
;; (C) 2002 sh0dan
;; Optimizations (C) ARDA
;; port to nasm by Kurosu (kurosu@inforezo.org)

;; This program is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 2 of the License, or
;; (at your option) any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program; if not, write to the Free Software
;; Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA, or visit
;; http://www.gnu.org/copyleft/gpl.html .
;;
;; Linking Avisynth statically or dynamically with other modules is making a
;; combined work based on Avisynth.  Thus, the terms and conditions of the GNU
;; General Public License cover the whole combination.

BITS 32

%macro cglobal 2
  %ifdef PREFIX
    %ifdef SUFFIX
      global _%1@%2
      %define %1 _%1@%2
    %else
      global _%1
      %define %1 _%1
    %endif
  %else
    global %1
  %endif
%endmacro

;=============================================================================
; Read only data
;=============================================================================

SECTION .rodata

ALIGN 16
rounder: dd  0x00004000, 0x00004000

;=============================================================================
; Declaration
;=============================================================================
SECTION .text
cglobal blender_mmx_nasm, 28

;=============================================================================
; Code
;=============================================================================
SECTION .code

ALIGN 16
blender_mmx_nasm:
  push       ebp
  push       ebx
  push       eax
  push       ecx
  push       edx
  push       edi
  push       esi

  mov        edi, [esp+20+ 0]		; dst.ptr
  mov        ebp, [esp+20+ 4]		; dst.pitch
  mov        esi, [esp+20+ 8]		; src.ptr
  mov        edx, [esp+20+12]		; src.pitch
  mov        ebx, [esp+20+16]		; dst.width
  mov        ecx, [esp+20+20]		; dst.height
  movd       mm7, [esp+20+24]		; weight
  movq       mm5, [rounder]		; 0x00000004 | 0x00000004
  punpckldq  mm7, mm7			; weight | weight
  pxor       mm6, mm6

ALIGN 16
.yloop:
  xor        eax, eax

ALIGN 16
.xloop:
  punpcklbw  mm0, [edi + eax]		; 4 pixels
   pxor      mm3, mm3
  punpcklbw  mm1, [esi + eax]		; y300 y200 y100 y000
   psrlw     mm0, 8			; 00y3 00y2 00y1 00y0
  psrlw      mm1, 8			; 00y3 00y2 00y1 00y0  
   pxor      mm2, mm2
  movq       mm4, mm1
   punpcklwd mm2, mm0
  punpckhwd  mm3, mm0  
   punpcklwd mm4, mm6
  punpckhwd  mm1, mm6
   por mm2,  mm4
  por mm3,   mm1
   pmaddwd   mm2, mm7			; Multiply pixels by weights.
					; pixel = i1*w + i2*invweight (twice)
  pmaddwd    mm3, mm7			; Stalls 1 cycle (multiply unit stall)
   paddd     mm2, mm5			; Add rounder
  paddd      mm3, mm5
   psrld     mm2, 15			; Shift down, so there is no fraction.
  psrld      mm3, 15        
  packssdw   mm2, mm3
  packuswb   mm2, mm6 
  movd       [edi+eax], mm2

  add        eax, 4
  cmp        ebx, eax
  jg         .xloop

  add        esi, ebp;
  add        edi, edx;
  dec        ecx
  jnz        .yloop

  emms
  push       edx
  push       edx
  push       edx
  push       edx
  push       edi
  push       esi
  push       ebp
  ret
