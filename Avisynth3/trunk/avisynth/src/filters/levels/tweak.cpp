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
#include "tweak.h"
#include "../../core/videoinfo.h"
#include "../../core/colorspace.h"
#include "../../core/videoframe.h"
#include "../../core/bufferwindow.h"
#include "../../core/cow_shared_ptr.h"
#include "../../core/runtime_environment.h"

//stl include
#include <math.h>                   //for sin, cos


namespace avs { namespace filters {



Tweak::Tweak(PClip const& child, double hue, double sat, double bright, double cont) 
  : clip::onechild::Concrete( child )
  , clip::caching::Concrete( child->GetEnvironment() )
  , Sat( int(sat * 512) )
  , Bright_p16( int(bright) + 16 )
  , Cont( int(cont * 512) )
{
  double Hue = (hue * 3.1415926) / 180.0;

  Sin = int(sin(Hue) * 4096);
  Cos = int(cos(Hue) * 4096);
}


Tweak::Tweak(PClip const& child, Tweak const& other)
  : clip::onechild::Concrete( child )
  , clip::caching::Concrete( child->GetEnvironment() )
  , Sin( other.Sin )
  , Cos( other.Cos )
  , Sat( other.Sat )
  , Bright_p16( other.Bright_p16 )
  , Cont( other.Cont ) { }


PClip Tweak::clone(PClip const& child) const
{
  ColorSpace& space = child->GetVideoInfo()->GetColorSpace();

  switch( space.id() )
  {
  case ColorSpace::I_YUY2: return PClip( (Clip *)new YUY2(child, *this) );
  case ColorSpace::I_YV12: return PClip( (Clip *)new YV12(child, *this) );
  }

  space.ThrowUnsupportedColorSpaceException();
}


PClip Tweak::Create(PClip child, double hue, double sat, double bright, double cont)
{
  ColorSpace& space = child->GetVideoInfo()->GetColorSpace();

  switch( space.id() )
  {
  case ColorSpace::I_YUY2: return PClip(new YUY2(child, hue, sat, bright, cont));
  case ColorSpace::I_YV12: return PClip(new YV12(child, hue, sat, bright, cont));
  }

  space.ThrowUnsupportedColorSpaceException();
}


CPVideoFrame Tweak::YUY2::MakeFrame(CPVideoFrame const& source) const
{

  PVideoFrame frame = source;
  WindowPtr wp = frame->WriteTo(NOT_PLANAR);

    
  for ( int y = wp.height; y-- > 0; wp.to(0, 1) )		
  {
    BYTE * ptr = wp.ptr;

    for ( int x = wp.width >> 2; x-- > 0; ptr += 4 )			
    {				
      // brightness and contrast 
	  	int	y1 = ptr[0] - 16;
		  int y2 = ptr[2] - 16;
				
      y1 = (Cont * y1) >> 9;
  		y2 = (Cont * y2) >> 9;
			
      ptr[0] = std::min(std::max(0, y1 + Bright_p16), 255);
		  ptr[2] = std::min(std::max(0, y2 + Bright_p16), 255);


 			// hue and saturation 
  		int	u = ptr[1] - 128;
	  	int	v = ptr[3] - 128;

		  int	ux = (+ u * Cos + v * Sin) >> 12;
 			int vx = (- u * Sin + v * Cos) >> 12;			

      u = ((ux * Sat) >> 9) + 128;
 			v = ((vx * Sat) >> 9) + 128;
	  	
      ptr[1] = std::min(std::max(0, u), 255);
	  	ptr[3] = std::min(std::max(0, v), 255);
    }		
  }

  return frame;
}


CPVideoFrame Tweak::YUY2::ISSE::MakeFrame(CPVideoFrame const& source) const
{

  PVideoFrame frame = source;
  WindowPtr wp = frame->WriteTo(NOT_PLANAR);

  	
  static const __int64 norm = 0x0080000000800000;

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


CPVideoFrame Tweak::YV12::MakeFrame(CPVideoFrame const& source) const
{

  PVideoFrame frame = source;

  WindowPtr Y = frame->WriteTo(PLANAR_Y);
  WindowPtr U = frame->WriteTo(PLANAR_U);
  WindowPtr V = frame->WriteTo(PLANAR_V);

  const Dimension dim = frame->GetDimension();


  for( int y = Y.height; y-- > 0; Y.to(0, 1) )
  {
    BYTE * ptr = Y.ptr;

    for( int x = Y.width; x-- > 0; ++ptr )
    {
      // brightness and contrast 
      int y = *ptr - 16;
      y = (Cont * y) >> 9;
      y += Bright_p16;
      *ptr = std::min( std::max(15, y), 235);
    }
  }


  for( y = U.height; y-- > 0; U.to(0, 1), V.to(0, 1) )
  {
    BYTE * pu = U.ptr;
    BYTE * pv = V.ptr;

    for ( int x = U.width; x-- > 0; ++pu, ++pv )
    {
      // hue and saturation 
			int u = *pu - 128;
			int	v = *pv - 128;

			int	ux = (+ u * Cos + v * Sin) >> 12;
      int vx = (- u * Sin + v * Cos) >> 12;
				
			u = ((ux * Sat) >> 9) + 128;
			v = ((vx * Sat) >> 9) + 128;
			
      *pu = std::min( std::max(16, u), 240 );
      *pv = std::min( std::max(16, v), 240 );				      
    }
  }

  return frame;
}





} } //namespace avs::filters