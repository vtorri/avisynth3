;; Avisynth v3.0 alpha.  Copyright 2004 David Pierre - Ben Rudiak-Gould et al.
;; http://www.avisynth.org
;; Original author unknwon - port to nasm by Kurosu (kurosu@inforezo.org)

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
norm: dw  128, 0, 128, 0

;=============================================================================
; Declaration
;=============================================================================
SECTION .text
cglobal tweak_yuy2_isse_nasm, 40

;=============================================================================
; Code
;=============================================================================
SECTION .code

;-----------------------------------------------------------------------------
; You're welcome to change the way values are loaded, like for instance doing
; yourself the int16->int64 unpacking
;-----------------------------------------------------------------------------
ALIGN 16
tweak_yuy2_isse_nasm:
  push      ebx
  push      esi
  push      edi
  mov       eax, [esp+12+ 4]	; wp.ptr
  mov       edx, [esp+12+ 8]	; y
  mov       ebx, [esp+12+12]	; x
  mov       edi, [esp+12+16]	; modulo
  pxor      mm0, mm0
  movq      mm1, [norm]		    ; 128 0 128 0
  movq      mm2, [esp+12+20]	; hue64 = Cos -Sin Sin Cos (fix12)
  movq      mm3, [esp+12+28]	; satcont64 = Sat Cont Sat Cont (fix9)
  movq      mm4, mm1		    ; bright64
  paddw     mm4, [esp+12+36]	; 128 Bright_p16 128 Bright_p16

ALIGN 16
.y_loop:
  mov       ecx, ebx          	; width

ALIGN 16
.x_loop:
  movd      mm7, [esi]		; 0000VYUY
  punpcklbw mm7, mm0
  psubw	    mm7, mm1		; V Y U Y
  pshufw    mm6, mm7, 0xDD	; V U V U
  pmaddwd   mm6, mm2		; V*Cos-U*Sin V*Sin+U*Cos (fix12)
  psrad	    mm6, 12		    ; ? V' ? U'
  movq      mm5, mm7
  punpcklwd mm7, mm6		; ? ? U' Y
  punpckhwd mm5, mm6		; ? ? V' Y
  punpckldq mm7, mm5		; V' Y U' Y
  psllw	    mm7, 7		    ; (fix7)
  pmulhw    mm7, mm3		; V'*Sat Y*Cont U'*Sat Y*Cont
  paddw     mm7, mm4		; V" Y" U" Y"
  packuswb  mm7, mm0		; 0000V"Y"U"Y"
  movd	    [esi], mm7

  add       esi, 4
  dec       ecx
  jnz       .x_loop

  add       esi, edi    	; skip to next scanline
  dec       edx
  jnz       .y_loop

  emms
  pop       edi
  pop       esi
  pop       ebx
  ret