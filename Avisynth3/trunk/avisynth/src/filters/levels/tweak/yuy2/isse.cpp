// Avisynth v3.0 alpha.  Copyright 2004 Ben Rudiak-Gould et al.
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
#include "isse.h"
#include "../../../../core/videoframe.h"
#include "../../../../core/cow_shared_ptr.h"


namespace avs { namespace filters { namespace tweak { namespace yuy2 {



CPVideoFrame ISSE::MakeFrame(CPVideoFrame const& source) const
{

  PVideoFrame frame = source;
  WindowPtr wp = frame->WriteTo(NOT_PLANAR);

  	
  static __int64 const norm = 0x0080000000800000;

  int y = wp.height;
  int x = wp.width >> 2;   //2 pixels (4 bytes) per x loop

  int modulo = wp.padValue();

 	__int64 hue64 = (__int64(Cos)<<48) + (__int64(-Sin)<<32) + (__int64(Sin)<<16) + __int64(Cos);
	__int64 satcont64 = (__int64(Sat)<<48) + (__int64(Cont)<<32) + (__int64(Sat)<<16) + __int64(Cont);
	__int64 bright64 = (__int64(Bright_p16)<<32) + __int64(Bright_p16);

	 __asm
   {

  	pxor		   mm0, mm0
	 	movq		   mm1, norm				  // 128 0 128 0
	  movq		   mm2, hue64     		// Cos -Sin Sin Cos (fix12)
  	movq		   mm3, satcont64 		// Sat Cont Sat Cont (fix9)
	 	movq		   mm4, mm1
	  paddw		   mm4, bright64  		// 128 Bright_p16 128 Bright_p16

  	mov			   esi, wp.ptr    		
	 	mov			   edx, y         		// height
  
  y_loop:
	
    mov			   ecx, x	          	// width

  x_loop:
	
    movd		   mm7, [esi]   			// 0000VYUY
	  punpcklbw	 mm7, mm0
 		psubw		   mm7, mm1				    //  V Y U Y
  	pshufw		 mm6, mm7, 0xDD			//  V U V U
	  pmaddwd		 mm6, mm2				    // V*Cos-U*Sin V*Sin+U*Cos (fix12)
	  psrad		   mm6, 12					  // ? V' ? U'
 		movq		   mm5, mm7
  	punpcklwd	 mm7, mm6				    // ? ? U' Y
	  punpckhwd	 mm5, mm6				    // ? ? V' Y
 		punpckldq	 mm7, mm5				    // V' Y U' Y
  	psllw		   mm7, 7					    // (fix7)
	  pmulhw		 mm7, mm3	          // V'*Sat Y*Cont U'*Sat Y*Cont
 		paddw		   mm7, mm4				    // V" Y" U" Y"
  	packuswb	 mm7, mm0				    // 0000V"Y"U"Y"
	  movd		   [esi], mm7

 		add			   esi, 4
  	dec			   ecx
	  jnz			   x_loop

 		add			   esi, modulo    		// skip to next scanline
  	dec			   edx
	  jnz			   y_loop

 		emms
 	}

  return frame;
}


  
} } } } //namespace avs::filters::tweak::yuy2

