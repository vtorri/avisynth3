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
#include "rgb.h"
#include "yuy2.h"


namespace avs { namespace vframe {



YUY2::YUY2(RGB32 const& source)
  : interleaved<2, 1>( ColorSpace::yuy2(), source )
{

  CWindowPtr src = source.GetMain().Read();
  WindowPtr dst = GetMain().Write();

  src.to(0, src.height - 1);           //rgb is upside down


	/////////////////////////////////////////////////////////////////////////////////////////
	// MMX code by Klaus Post
	// - Notes on MMX:
	// Fractions are one bit less than integer code,
	//  but otherwise the algorithm is the same, except
	//  r_y and b_y are calculated at the same time.
	// Order of execution has been changed much for better pairing possibilities.
	// It is important that the 64bit values are 8 byte-aligned
	//  otherwise it will give a huge penalty when accessing them.
  // Instructions pair rather ok, instructions from the top is merged
	//  into last part, to avoid dependency stalls.
	//  (paired instrucions are indented by a space)
	//
  // Modifications by David Pierre :
  //   - removed entry test, 3.0 guarantees there are pixels to work on
  //   - process 4 pixels per xloop (we may make extra work if width != 0 mod4)
  //   - almost perfect pairing now (just a couple artificial pair to lessen mem access) 

	__declspec(align(8)) static __int64 const yRounder  = 0x0008400000084000; // 16.5<<16=0x108000  /2 = 0x84000
	__declspec(align(8)) static __int64 const yyMult    = 0x4A854A854A854A85; // int(255.0 / 219.0 * 32768 + 0.5) = 0x4A85
	__declspec(align(8)) static __int64 const yyMult32  = 0x000950A0000950A0; // 32*int(255.0 / 219.0 * 32768 + 0.5) = 0x950A0
	__declspec(align(8)) static __int64 const uvRounder = 0x0080800000808000; // 128.5<<16=0x808000
	// int(1 / 2.018 * 1024 + 0.5) = 0x01FB
  // int(1 / 1.596 * 1024 + 0.5) = 0x0282
  __declspec(align(8)) static __int64 const uvMult    = 0x00000282000001FB;
  
  // cyb = int(0.114 * 219 / 255 * 32768 + 0.5) = 0x0c88
  // cyg = int(0.587 * 219 / 255 * 32768 + 0.5) = 0x4087
  // cyr = int(0.299 * 219 / 255 * 32768 + 0.5) = 0x20DE
	__declspec(align(8)) static __int64 const cybgr     = 0x000020DE40870c88;


  int y = src.height;                 //y loop counter
  int xcount = (dst.width + 4) / 8;   //we do 4 pixels per xloop

  int src_pad = -16*xcount - src.pitch;   //back to start then next line (neg coz rgb upside down)
  int dst_pad = dst.pitch - 8 * xcount;   //back to start then next line

	__asm 
  {
	  mov         esi, src.ptr          //esi points to src
		mov         edi, dst.ptr          //edi points to dst
    
    sub         edi, 8                //edi -= 8  later we add to edi before writing to

    align 16

  yloop:

    mov         ecx, xcount           //x loop counter

		movq        mm0, [esi]		        //mm0 = XXR1 G1B1 XXR0 G0B0
     pxor       mm6, mm6              //mm6 = 0
    movq        mm2, [esi + 8]        //mm2 = XXR3 G3B3 XXR2 G2B2
	   punpckhbw  mm1, mm0              //mm1 = XXXX R1XX G1XX B1XX
    movq        mm7, cybgr            //mm7 = 0000  cyr  cyg  cyr
     punpcklbw  mm0, mm6              //mm0 = 00XX 00R0 00G0 00B0
    movq        mm4, mm0              //mm4 = 00XX 00R0 00G0 00B0
     psrlw      mm1, 8                //mm1 = 00XX 00R1 00G1 00B1

    align 16

  xloop:

    paddw       mm4, mm1              //mm4 = 0XXX R0+1 0XXX B0+1
     punpckhbw  mm3, mm2              //mm3 = XXXX R3XX G3XX B3XX	
    pmaddwd     mm0, mm7              //mm0 =  cyr*R0 cyg*G0+cyb*B0    
		 punpcklbw  mm2, mm6              //mm2 = 00XX 00R2 00G2 00B2
    pmaddwd     mm1, mm7              //mm1 =  cyr*R1 cyg*G1+cyb*B1
     pslld      mm4, 14               //mm4 = R0+R1<<14 B0+B1<<14    XXX got killed  (was 15 in C)
    movq        mm5, mm2              //mm5 = 00XX 00R2 00G2 00B2
     psrlw      mm3, 8                //mm3 = 00XX 00R3 00G3 00B3
    pmaddwd     mm2, mm7              //mm2 =  cyr*R2 cyg*G2+cyb*B2
     paddw      mm5, mm3              //mm5 = 0XXX R2+3 0XXX B2+3
    movq        mm6, mm0              //mm6 =  cyr*R0 cyg*G0+cyb*B0  no stall
     pslld      mm5, 14               //mm5 = R2+R3<<14 B2+B3<<14    XXX got killed  (was 15 in C)
    pmaddwd     mm3, mm7              //mm1 =  cyr*R3 cyg*G3+cyb*B3  
     punpckldq  mm0, mm1              //mm0 = g*G1+b*B1 g*G0+b*B0
    paddd       mm0, yRounder
     punpckhdq  mm6, mm1              //mm6 =   cyr*R1   cyr*R0
    paddd       mm0, mm6              //mm0 =   Y1<<15   Y0<<15
     add        esi, 16               //esi += 16  next src pixels
    movq        mm7, mm2              //mm7 =  cyr*R2 cyg*G2+cyb*B2  no stall
     psrld      mm0, 15               //mm0 = 0000 00Y1 0000 00Y0
    movq        mm1, mm0              //mm1 = 0000 00Y1 0000 00Y0
     punpckldq  mm2, mm3              //mm2 = g*G3+b*B3 g*G2+b*B2
    paddd       mm2, yRounder
     packssdw   mm1, mm1              //mm1 = 00Y1 00Y0 00Y1 00Y0
    pmaddwd     mm1, yyMult           //mm1 = 00 YYY01  00 YYY01    24 bits not zeros (max)
     punpckhdq  mm7, mm3              //mm7 =   cyr*R3   cyr*R2 
    paddd       mm4, yyMult32         //mm4 = 00 RRR01  00 BBB01    24 bits not zeros (max)
     paddd      mm2, mm7              //mm2 =   Y3<<15   Y2<<15
    paddd       mm5, yyMult32         //mm4 = 00 RRR23  00 BBB23    24 bits not zeros (max)
     psrld      mm2, 15               //mm2 = 0000 00Y3 0000 00Y2
    movq        mm3, mm2              //mm3 = 0000 00Y3 0000 00Y2
     packssdw   mm0, mm2              //mm0 = 00Y3 00Y2 00Y1 00Y0
    pmaddwd     mm3, yyMult           //mm3 = 00 YYY23  00 YYY23    24 bits not zeros (max)
     psubd      mm4, mm1              //mm4 = ss 'r_y'  ss 'b_y'    ss is either 00 or FF (sign)
    movq        mm1, uvMult           //mm1 = uvMult
     psrad      mm4, 9                //mm4 = ssss sr_y ssss sb_y   1 sign bit got through
    pmaddwd     mm4, mm1              // * uvMult
     add        edi, 8                //edi += 8   next dst pixels
    psubd       mm5, mm3              //mm5 = ss 'r_y'  ss 'b_y'    ss is either 00 or FF (sign)
     dec        ecx                   //--ecx
    movq        mm2, uvRounder        //mm2 = uvRounder
     psrad      mm5, 9                //mm5 = ssss sr_y ssss sb_y   1 sign bit got through
    pmaddwd     mm5, mm1              // * uvMult
     paddd      mm4, mm2              //mm4 = VV01 XXXX UU01 XXXX
    movq        mm3, mm0              //mm3 = 00Y3 00Y2 00Y1 00Y0
     psrld      mm4, 16               //mm4 = 0000 VV01 0000 UU01
    movq        mm0, [esi]            //mm0 = XXR1 G1B1 XXR0 G0B0 from top for better pairing
     pxor       mm6, mm6              //mm6 = 0                   from top for better pairing
    paddd       mm5, mm2              //mm5 = VV23 XXXX UU23 XXXX
	   punpckhbw  mm1, mm0              //mm1 = XXXX R1XX G1XX B1XX from top for better pairing
    movq        mm2, [esi + 8]        //mm2 = XXR3 G3B3 XXR2 G2B2 from top for better pairing
     psrld      mm5, 16               //mm5 = 0000 VV23 0000 UU23
    movq        mm7, cybgr            //mm7 = 0000  cyr  cyg  cyr from top for better pairing
     packuswb   mm4, mm5              //mm4 = 00Vb 00Ub 00Va 00Ua
    movq        mm5, mm4              //mm5 = 00Vb 00Ub 00Va 00Ua
     punpcklbw  mm0, mm6              //mm0 = 00XX 00R0 00G0 00B0 from top for better pairing
    movq        mm4, mm0              //mm4 = 00XX 00R0 00G0 00B0 from top for better pairing
     psllw      mm5, 8                //mm5 = Vb00 Ub00 Va00 Ua00
    por         mm5, mm3              //mm5 = VbY3 UbY2 VaY1 UaY0
     psrlw      mm1, 8                //mm1 = 00XX 00R1 00G1 00B1 from top for better pairing
    movq        [edi], mm5            //write to dst
     jnz        xloop
    
    add        esi, src_pad           //move to next src line
    add        edi, dst_pad           //move to next dst line

    dec        y                      //--y
    jnz        yloop

    emms
  }
}





} } //namespace avs::vframe