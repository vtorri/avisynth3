;; Avisynth v3.0 alpha.  Copyright 2004 David Pierre - Ben Rudiak-Gould et al.
;; http://www.avisynth.org
;; Horizontal resize assembly code (C) 2004 David Pierre
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
; Symbols
;=============================================================================
SECTION .text
cglobal resize_horizontal_rgb24_mmx_nasm, 32
cglobal resize_horizontal_rgb24_mmx_nasm, 36
cglobal resize_horizontal_rgb24_mmx_nasm, 36
cglobal emms_, 0

;=============================================================================
; Code
;=============================================================================
SECTION .code

; void emms(void)
emms_:
    emms
    ret

;-----------------------------------------------------------------------------
; void resize_horizontal_rgb24_mmx_nasm(const BYTE *src_ptr,
;                    BYTE *dst_ptr,
;                    int dst_width,
;                    int const *pptr,
;                    int count,
;                    int y,
;                    int src_pitch,
;                    int pad)
;-----------------------------------------------------------------------------
ALIGN 16
resize_horizontal_rgb24_mmx_nasm:
    push        ebp
    push        ebx
    push        esi
    push        edi
    mov         esi, [esp+20+ 4]	    ; src.ptr
    mov         edi, [esp+20+ 8]	    ; dst.ptr
    mov         ebp, [esp+20+24]	    ; y

    pcmpeqd     mm6, mm6                ; mm6 = FFFFFFFFFFFFFFFF
    pxor        mm7, mm7                ; mm7 = 0
    psrld       mm6, 19                 ; mm6 = 00001FFF00001FFF    13 bits rounder

ALIGN 16
.yloop:
    mov         ecx, [esp+20+12]        ; ecx = dst.width  (x loop counter)
    mov         edx, [esp+20+16]        ; edx points into the pattern                 

ALIGN 16
.xloop:
    mov         ebx, [edx]              ; read from pattern: ebx = ref pixel offset
    mov         eax, [esp+20+20]        ; eax = count  (a loop counter)
    lea         ebx, [ebx + 2*ebx]      ; ebx *= 3  byte offset to the ref pixel
    add         edx, 4                  ; edx += 4  now points to the coeffs 

    movq        mm0, mm6                ; mm0 = mm6  ( G total | B total )  init rounded
    movq        mm1, mm6                ; mm1 = mm6  ( X total | R total )  init rounded

ALIGN 16
.aloop:
    movd        mm2, [esi + ebx]        ; mm2 = 0|0|0|0|x|r|g|b  read src pixel
    punpcklbw   mm2, mm7                ; mm2 = 0x|0r|0g|0b
    movq        mm3, mm2                ; mm3 = 0x|0r|0g|0b
    punpcklwd   mm2, mm7                ; mm2 = 00|0g|00|0b
    punpckhwd   mm3, mm7                ; mm3 = 00|0x|00|0r
    movd        mm5, [edx]              ; mm5 = 00|00|coeff 
    packssdw    mm5, mm5                ; mm5 = 00|co|00|co  coeff saturated (useless if always positive: to check)
    pmaddwd     mm2, mm5                ; mm2 =  g*co|b*co
    add         edx, 4                  ; edx += 4  next coeff
    pmaddwd     mm3, mm5                ; mm3 =  x*co|r*co
    add         ebx, 3                  ; ebx += 3  next src pixel
    dec         eax                     ; --eax
    paddd       mm0, mm2                ; accumulates g and b
    paddd       mm1, mm3                ; accumulates r (and x)

    jnz         .aloop                  ; jump if eax != 0

    psrld       mm0, 14                 ; mm0 = 00|0g|00|0b    kills 14 bit fraction
    psrld       mm1, 14                 ; mm1 = 00|0x|00|0r    kills 14 bit fraction
    packuswb    mm0, mm1                ; mm0 = 0|x|0|r|0|g|0|b
    packuswb    mm0, mm2                ; mm0 = 0|0|0|0|x|r|g|b
    movd        [edi], mm0              ; write an extra x in memory, but we don't care
                                        ; next write will overwrite it by a significant value
    add         edi, 3
    sub         ecx, 3
    jnz         .xloop

    add         esi, [esp+20+28]        ; move to next src
    add         edi, [esp+20+32]        ; move to next dst line

    dec         ebp                     ; --y
    jnz         .yloop

    emms
    pop         edi
    pop         esi
    pop         ebx
    pop         ebp

    ret


;-----------------------------------------------------------------------------
; void resize_horizontal_yuy2_mmx_nasm(const BYTE *src_ptr,
;                   int src_width,
;                   BYTE *tempY,
;                   BYTE *tempUV,
;                   const int *pptrUV,
;                   const int *pptrY,
;                   int countY,
;                   int countUV,
;                   int x)
;-----------------------------------------------------------------------------
ALIGN 16
resize_horizontal_yuy2_mmx_nasm:
    push        ebp
    push        ebx
    push        esi
    push        edi
    xor         eax, eax                ; eax = 0
    mov         ebx, [esp+20+ 4]	; src.ptr
    mov         ecx, [esp+20+12]        ; src.width
    mov         esi, [esp+20+16]        ; tempY
    mov         edi, [esp+20+20]        ; tempUV

    pcmpeqw     mm7, mm7                ; mm7 = FFFFFFFFFFFFFFFF
    psrlw       mm7, 8                  ; mm7 = 00FF00FF00FF00FF   Y mask

ALIGN 16
.deintloop:
    movq        mm2, [ebx + 8*eax]      ; mm2 = VYUY|VYUY
    pxor        mm4, mm4                ; mm4 = 0
    movq        mm3, mm2                ; mm3 = VYUY|VYUY
    pand        mm2, mm7                ; mm2 = 0Y0Y0Y0Y
    psrlw       mm3, 8                  ; mm5 = Vb|Ub|Va|Ua
    movq        [esi + 8*eax], mm2      ; writes to tempY      
    punpckhwd   mm4, mm3                ; mm4 = Vb| 0|Ub| 0
    punpcklwd   mm3, mm0                ; mm3 =  0|Va| 0|Ua
    por         mm3, mm4                ; mm3 = Vb|Va|Ub|Ua
    movq        [edi + 8*eax], mm3      ; writes to tempUV
  
    inc         eax                     ; ++eax
    dec         ecx                     ; --ecx
    jnz         .deintloop

    mov         edx, [esp+20+24]        ; edx points in chroma pattern
    mov         edi, [esp+20+28]        ; edi points in luma pattern

ALIGN 16
.xloop:
    mov         ebp, [esp+20+32]        ; aloop counter on stack
    push        edx                     ; save edx (chroma pattern) on stack

    mov         eax, [edi]              ; eax = offset0
    mov         ebx, [edi + 4]          ; ebx = offset1
    mov         ecx, [edi + 8]          ; ecx = offset2
    mov         edx, [edi + 12]         ; edx = offset3
    mov         esi, [esp+20+16]	; esi = tempY
    add         edi, 16                 ; edx += 16 now points to the coeffs

    pcmpeqd     mm0, mm0                ; mm0 = FFFFFFFFFFFFFFFF
    psrlq       mm0, 51                 ; mm0 = 0000000000001FFF    init rounded
    movq        mm1, mm0                ; mm1 = 0000000000001FFF    init rounded
    movq        mm2, mm0                ; mm2 = 0000000000001FFF    init rounded
    movq        mm3, mm0                ; mm3 = 0000000000001FFF    init rounded

ALIGN 16
.aloopY:
    movq        mm4, [esi + 2*eax]      ; mm4 = Yd|Yc|Yb|Ya
    movq        mm5, [esi + 2*ebx]      ; mm5 = Yi|Yh|Yg|Yf
    movq        mm6, [esi + 2*ecx]      ; mm6 = Yp|Yo|Yn|Ym
    movq        mm7, [esi + 2*edx]      ; mm7 = Yv|Yu|Yt|Ys
    pmaddwd     mm4, [edi]              ; mm4 =  Y0b | Y0a    [edi]    = Cd|Cc|Cb|Ca
    pmaddwd     mm5, [edi + 8]          ; mm5 =  Y1b | Y1a  [edi + 8]  = Ci|Ch|Cg|Cf
    pmaddwd     mm6, [edi + 16]         ; mm6 =  Y2b | Y2a  [edi + 16] = Cp|Co|Cn|Cm
    pmaddwd     mm7, [edi + 24]         ; mm7 =  Y3b | Y3a  [edx + 24] = Cv|Cu|Ct|Cs
    paddd       mm0, mm4                ; accumulate Y0
    paddd       mm1, mm5                ; accumulate Y1
    paddd       mm2, mm4                ; accumulate Y2
    paddd       mm3, mm5                ; accumulate Y3

    add         edi, 32                 ; edx += 32  next coeffs
    add         esi, 8                  ; esi += 8   next src Y

    dec         ebp                     ; --aloop counter
    jnz         .aloopY

    pop         edx                     ; restore edx (chroma pattern) from stack 

    mov         eax, [edx]              ; eax = offset0
    mov         ebx, [edx + 4]          ; ebx = offset1
    mov         ecx, [esp+20+36]	; ecx = countUV
    add         edx, 8                  ; edx += 8  now points to coeffs
    mov         esi, [esp+20+20]	; esi = tempUV

    pcmpeqd     mm4, mm4                ; mm4 = FFFFFFFFFFFFFFFF
    psrld       mm4, 19                 ; mm4 = 00001FFF00001FFF   init rounded
    movq        mm5, mm4                ; mm5 = 00001FFF00001FFF   init rounded

ALIGN 16
.aloopUV:
    movq        mm6, [edx]              ; mm6 = Cn|Cm|Cb|Ca
    movq        mm7, mm6                ; mm7 = Cn|Cm|Cb|Ca
    punpcklwd   mm6, mm6                ; mm6 = Cb|Ca|Cb|Ca
    punpcklwd   mm7, mm7                ; mm7 = Cn|Cm|Cn|Cm
    pmaddwd     mm6, [esi + 4*eax]      ; mm6 =   V0 | U0   [esi + 4*eax] = Vb|Va|Ub|Ua
    pmaddwd     mm7, [esi + 4*ebx]      ; mm7 =   V1 | U1   [esi + 4*ebx] = Vn|Vm|Un|Um
    paddd       mm4, mm6                ; accumulate U0 V0
    paddd       mm5, mm7                ; accumulate U1 V1

    add         edx, 8                  ; edi += 8  next coeffs
    add         esi, 8                  ; esi += 8  next src UV

    dec         ecx                     ; --ecx
    jnz         .aloopUV


    movq        mm6, mm0                ; mm6 =  Y0b | Y0a
    pslld       mm4, 2                  ; mm4 =  V0--|U0--   compensate 14 bits fractional
    movq        mm7, mm2                ; mm7 =  Y2b | Y2a
    pslld       mm5, 2                  ; mm5 =  V1--|U1--   compensate 14 bits fractional
    punpckhwd   mm6, mm1                ; mm6 =  Y1b | Y0b
    packuswb    mm4, mm5                ; mm4 =  v-u-|v-u-   (- is garbage)
    punpckhwd   mm7, mm3                ; mm7 =  Y3b | Y2b
    psrlw       mm4, 8                  ; mm4 =  0v0u|0v0u
    punpcklwd   mm0, mm1                ; mm0 =  Y1a | Y0a
    psllw       mm4, 8                  ; mm4 =  v0u0|v0u0
    punpcklwd   mm2, mm3                ; mm2 =  Y3a | Y2a
    paddd       mm0, mm6                ; mm0 =  Y1  | Y0
    paddd       mm2, mm7                ; mm2 =  Y3  | Y2

    pslld       mm0, 2                  ; mm0 =  Y1--|Y0--   compensate 14 bits fractional
    pslld       mm2, 2                  ; mm2 =  Y3--|Y2--   compensate 14 bits fractional
    packuswb    mm0, mm2                ; mm0 =  y-y-|y-y-
    psrlw       mm0, 8                  ; mm0 =  0y0y|0y0y  
    por         mm0, mm4                ; mm0 =  vyuy|vyuy

    mov         ebx, [esp+20+ 8]        ; ebx points to dst
    add         dword[esp+20+ 8], 8     ; dstp +=8   next pixel couple
    movq        [ebx], mm0              ; write result

    dec         dword[esp+20+40]        ; --x
    jnz         .xloop

    pop         edi
    pop         esi
    pop         ebx
    push        ebp
	
    ret

;-----------------------------------------------------------------------------
; void resize_horizontal_rgb32_mmx_nasm(const BYTE *src_ptr,
;                    int src_width,
;                    BYTE *tempY,
;                    BYTE *tempUV,
;                    const int *pptrUV,
;                    const int *pptrY,
;                    int countY,
;                    int countUV,
;                    int x)
;-----------------------------------------------------------------------------
ALIGN 16
resize_horizontal_rgb32_mmx_nasm:
    push        ebp
    push        ebx
    push        esi
    push        edi
    mov         esi, [esp+20+ 4]	; src.ptr
    mov         edi, [esp+20+ 8]	; dst.ptr
    mov         ebp, [esp+20+24]	; y

    pcmpeqd     mm6, mm6                ; mm6 = FFFFFFFFFFFFFFFF
    pxor        mm7, mm7                ; mm7 = 0
    psrld       mm6, 19                 ; mm6 = 00001FFF00001FFF    13 bits rounder

ALIGN 16
.yloop:
    mov         ecx, [esp+20+12]        ; ecx = dst.width  (x loop counter)
    mov         edx, [esp+20+16]        ; edx points into the pattern                 

ALIGN 16
.xloop:
    mov         eax, [esp+20+20]        ; eax = count  (a loop counter)
    mov         ebx, [edx]              ; read from pattern: ebx = ref pixel offset
    add         edx, 4                  ; edx += 4  now points to the coeffs 

    movq        mm0, mm6                ; mm0 = mm6  ( G total | B total )  init rounded
    movq        mm1, mm6                ; mm1 = mm6  ( X total | R total )  init rounded

ALIGN 16
.aloop:
    sub         edx, 4                  ; next coeff (descending)
    dec         eax                     ; next src pixel (descending)
    
    movd        mm2, [esi + 4*ebx]      ; mm2 =  0000|argb  read src pixel
    punpcklbw   mm2, mm7                ; mm2 = 0a|0r|0g|0b
    movq        mm3, mm2                ; mm3 = 0a|0r|0g|0b
    punpcklwd   mm2, mm7                ; mm7 = 00|0g|00|0b
    punpckhwd   mm3, mm7                ; mm6 = 00|0a|00|0r
    movd        mm5, [edx]              ; mm5 = 00|00|coeff 
    packssdw    mm5, mm5                ; mm5 = 00|co|00|co  coeff saturated (useless if always positive: to check)
    pmaddwd     mm2, mm5                ; mm7 =  g*co|b*co
    add         edx, 4                  ; edx += 4  next coeff
    pmaddwd     mm3, mm5                ; mm6 =  a*co|r*co
    inc         ebx                     ; ++ebx    next src pixel
    dec         eax                     ; --eax
    paddd       mm0, mm2                ; accumulates g and b
    paddd       mm1, mm3                ; accumulates a and r

    jnz         .aloop                  ; jump if eax != 0

    psrld       mm0, 14                 ; mm0 = 00|0g|00|0b    kills 14 bit fraction
    psrld       mm1, 14                 ; mm1 = 00|0a|00|0r    kills 14 bit fraction
    packuswb    mm0, mm1                ; mm0 = 0|a|0|r|0|g|0|b
    packuswb    mm0, mm2                ; mm0 = 0|0|0|0|a|r|g|b
    movd        [edi], mm0              ; write to memory

    add         edi, 4
    sub         ecx, 4
    jnz         .xloop

    add         esi, [esp+20+24]        ; move to next src
    add         edi, [esp+20+28]        ; move to next dst line

    dec         ebp                     ; --y
    jnz         .yloop

    emms
    ret