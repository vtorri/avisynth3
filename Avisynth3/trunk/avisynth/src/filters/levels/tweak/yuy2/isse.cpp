// Avisynth v3.0 alpha.  Copyright 2005 David Pierre - Ben Rudiak-Gould et al.
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
#include "../../../../define.h"                  //for AVS_HAS_INTEL_INLINE_ASM
#include "../../../../user_config.h"             //for AVS_ALWAYS_USE_NASM
#include "../../../../core/videoframe.h"
#include "../../../../core/cow_shared_ptr.h"


namespace avs { namespace filters { namespace tweak { namespace yuy2 {


//equivalent nasm code of the inline asm below
extern "C" void tweak_yuy2_isse_nasm
    (BYTE * ptr, int x, int y, int modulo, long long hue64, long long satcont64, long long bright64);



CPVideoFrame ISSE::MakeFrame(CPVideoFrame const& source) const
{

  PVideoFrame frame = source;
  WindowPtr wp = frame->WriteTo('~');
  
  long long hue64 = static_cast<long long>(Cos) << 48 
                  | static_cast<long long>(-Sin) << 32
                  | static_cast<long long>(Sin) << 16
                  | static_cast<long long>(Cos);

  int satcont = Sat << 16 | Cont;
  int bright = Bright_p16;             //just put on the stack so the asm can take the value


#if defined(AVS_HAS_INTEL_INLINE_ASM) && ! defined(AVS_ALWAYS_USE_NASM)

  static long long const norm = 0x0080000000800000LL;

  int y = wp.height;
  int x = wp.width >> 2;          //2 pixels (4 bytes) per x loop
  
  int modulo = wp.padValue();

  __asm
  {
    pxor		   mm0, mm0
    movq		   mm1, norm			    // 128   0  128  0
    movq		   mm2, hue64     		// Cos -Sin Sin Cos (fix12)
    movd		   mm3, satcont   		//  0    0  Sat Cont (fix9)
    punpckldq  mm3, mm3           // Sat Cont Sat Cont (fix9)
    movq		   mm4, mm1
    paddd		   mm4, bright    		// 128   0  128 Bright_p16
    punpckldq  mm4, mm4           // 128 Bright_p16 128 Bright_p16

    mov			   esi, wp.ptr    		
    mov			   edx, y         		// height
    align 16
  
  y_loop:
	
    mov			   ecx, x	          	// width
    align 16

  x_loop:
	
    movd		   mm7, [esi]   		  // 0000VYUY
    punpcklbw	 mm7, mm0
    psubw		   mm7, mm1	          //  V Y U Y
    pshufw		 mm6, mm7, 0xDD		  //  V U V U
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

#else

  //use nasm code
  tweak_yuy2_isse_nasm(wp.ptr, wp.width>>2, wp.height, wp.padValue(), hue64, satcont, bright);

#endif //defined(AVS_HAS_INTEL_INLINE_ASM) && ! defined(AVS_ALWAYS_USE_NASM)

  return frame;
}

  
} } } } //namespace avs::filters::tweak::yuy2

