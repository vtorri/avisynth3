;; Avisynth v3.0 alpha.  Copyright 2005 David Pierre - Ben Rudiak-Gould et al.
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

	
%macro cglobal 1
 %ifdef PREFIX
  global _%1
  %define %1 _%1
 %else
  global %1
 %endif
%endmacro

;=============================================================================
; Data
;=============================================================================
SECTION .rodata
	oxooooooooooffffff:	DD 0x00FFFFFF, 0x0 
	oxooffffffoooooooo:	DD 0x0,        0x00FFFFFF

;=============================================================================
; Symbols
;=============================================================================
SECTION .text
cglobal convert_rgb32_to_rgb24_mmx_nasm
; cglobal emms_

;=============================================================================
; Code
;=============================================================================
SECTION .code

; void emms(void)
;emms_:
;emms
;ret


;-----------------------------------------------------------------------------
; void convert_rgb32_to_rgb24_mmx_nasm(
;                    const BYTE *src_ptr,	
;                    BYTE *dst_ptr,
;                    int dst_height,
;                    int width, 
;                    int dst_pitch,
;                    int src_pitch)
;-----------------------------------------------------------------------------

ALIGN 16
convert_rgb32_to_rgb24_mmx_nasm:
	push	ebp
	push	ebx
	push	esi
	push	edi
	mov	esi, [esp+20+4]		; src_ptr
	mov	edi, [esp+20+8]		; dst_ptr
	mov	eax, [esp+20+12]	; dst_height
	mov	ebp, [esp+20+16]	; width
	mov	edx, ebp
	and	edx, 3			; width [4]
	and	ebp, -4			; width - width [4]
	sal	ebp, 2			; (width - (width [4])) * 4
	movq	mm6, [oxooooooooooffffff]
	movq	mm7, [oxooffffffoooooooo]

ALIGN 16
.yloop:
	xor	ebx, ebx		; ebx = 0 src offset
	xor	ecx, ecx		; ecx = 0 dst offset

ALIGN 16
.xloop:
	movq	mm0, [ebx+esi]		; a1r1 g1b1 a0r0 g0b0
	movq	mm1, [ebx+esi+8]	; a3r3 g3b3 a2r2 g2b2

	movq	mm2, mm0		; a1r1 g1b1 a0r0 g0b0
	movq	mm3, mm1		; a3r3 g3b3 a2r2 g2b2

	pand	mm0, mm6		; 0000 0000 00r0 g0b0
	pand	mm1, mm6		; 0000 0000 00r2 g2b2

	pand	mm2, mm7		; 00r1 g1b1 0000 0000
	movq	mm4, mm1		; 0000 0000 00r2 g2b2

	psrlq	mm2, 8			; 0000 r1g1 b100 0000
	pand	mm3, mm7		; 00r3 g3b3 0000 0000

	psllq	mm4, 48			; g2b2 0000 0000 0000
	por	mm0, mm2		; 0000 r1g1 b1r0 g0b0

	psrlq	mm1, 16			; 0000 0000 0000 00r2
	por	mm0, mm4		; g2b2 r1g1 b1r0 g0b0

	psrlq	mm3, 24			; 0000 0000 r3g3 b300
	movq	[ecx+edi], mm0

	por	mm3, mm1		; 0000 0000 r3g3 b3r2
	add	ebx, 16			; next src pixel
	movd	[ecx+edi+8], mm3

	add	ecx, 12			; next dst pixel
	cmp	ebx, ebp
	jl	.xloop

	cmp	edx, 0
	je	.nocopy
	cmp	edx, 2
	je	.copy2
	cmp	edx, 1
	je	.copy1
;.copy3
	add	esi, ebx
	add	edi, ecx
	movsb
	movsb
	movsb
	inc	esi
	movsb
	movsb
	movsb
	inc	esi
	movsb
	movsb
	movsb
	sub	esi, ebx
	sub	edi, ecx
	sub	esi, 11
	sub	edi, 9
	jmp	.nocopy

ALIGN 16
.copy2
	add	esi, ebx
	add	edi, ecx
	movsb
	movsb
	movsb
	inc	esi
	movsb
	movsb
	movsb
	sub	esi, ebx
	sub	edi, ecx
	sub	esi, 7
	sub	edi, 6
	jmp	.nocopy

ALIGN 16
.copy1
	add	esi, ebx
	add	edi, ecx
	movsb
	movsb
	movsb
	sub	esi, ebx
	sub	edi, ecx
	sub	esi, 3
	sub	edi, 3

ALIGN 16
.nocopy
	add	edi, [esp+20+20]	; move to next dst line
	add	esi, [esp+20+24]	; move to next src line

	dec	eax			; --y
	jnz	.yloop

	emms
	pop	edi
	pop	esi
	pop	ebx
	pop	ebp

	ret