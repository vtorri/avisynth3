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
#include "blitter.h"
#include "vecteur.h"
#include "block/align.h"
#include "bufferwindow.h"


namespace avs {



BufferWindow::BufferWindow(Dimension const& dimension, PEnvironment const& env)
  : dimension_( dimension )
  , pitch_( block::AlignValue(width()) )
  , offset_( block::Align )
  , buffer_( env, pitch() * height() + block::Align * 2, true ) { }


void BufferWindow::swap(BufferWindow& other)
{
  dimension_.swap(other.dimension_);
  std::swap(pitch_, other.pitch_);
  std::swap(offset_, other.offset_);
  buffer_.swap(other.buffer_);
}



BYTE * BufferWindow::write()
{
  if ( ! buffer_.unique() )
  {
    BufferWindow buf(GetDimension(), GetEnvironment());

    Blitter::Get()(*this, buf);

    swap(buf);
  }

  return buffer_.get() + offset_;
}

void BufferWindow::ChangeSize(Vecteur const& topLeft, Vecteur const& bottomRight)
{

  Dimension dim(dimension_);
  
  //updating parameters
  dim += bottomRight - topLeft;  
  int offs = offset_ + topLeft % pitch_;
 
  //conditions where needed to reallocate the Buffer
  if ( ! block::IsAligned(offs)           //alignment requirement no longer met
    || dim.GetWidth() > pitch_            //width got bigger than pitch
    || offs < block::Align                //not enough toe room
    || offs + dim.GetHeight() * pitch_ + block::Align > buffer_.size() )  //not enough head room
  {
    BufferWindow result(dim, GetEnvironment());

    result.Copy(*this, -topLeft);         //copy data at the right place in the new window
    
    *this = result;                       //replace current by new one
  }
  else {             
    dimension_ = dim;                     //else commit updated parameters
    offset_ = offs;
  }

}



void BufferWindow::Copy(BufferWindow other, Vecteur const& coords)
{
  Vecteur topLeft = coords >> dimension_;
  Vecteur bottomRight = ( coords + other.dimension_ ) >> dimension_;

  Dimension toCopy = Dimension(bottomRight - topLeft);

  if (! toCopy.empty() )
    Blitter::Get()( other.Read() += topLeft - coords, Write() += topLeft, toCopy ); 
}





void BufferWindow::Blend8(BufferWindow const& other, float factor)
{  

  int weight = int(factor * 32767.0f);            //NB: it is the weight for other (plane)

  if ( this != &other && weight > 0 )             //otherwise nothing to do
    if ( weight >= 32767 )                        //full blend of other into self
      *this = other;                              //we replace this window by the other one
    else                                          //we have to work
    {

      WindowPtr dst = Write();
      CWindowPtr src = other.Read();

      weight = (weight << 16) + 32767 - weight;   //aka weight-other | weight-this

      __int64 weight64  = __int64(weight) | (__int64(weight) << 32); //WO | WT | WO | WT
      static __int64 const rounder = 0x0000400000004000;		         //(0.5)<<15 in each dword

      __asm
      {
        movq mm5, [rounder]
        pxor mm6, mm6
        movq mm7, [weight64]
        mov  ebx, [dst.width]
        mov  esi, [dst.ptr]
        mov  edi, [src.ptr]
        mov  ecx, [dst.height]

        align 16

      yloop:

        xor  eax, eax
        align 16

      xloop:

        punpcklbw mm0, [esi + eax]  // 4 pixels
         pxor mm3, mm3
        punpcklbw mm1, [edi + eax]  // y300 y200 y100 y000
         psrlw mm0, 8               // 00y3 00y2 00y1 00y0
        psrlw mm1, 8                // 00y3 00y2 00y1 00y0  
         pxor mm2, mm2
        movq mm4, mm1
         punpcklwd mm2, mm0
        punpckhwd mm3, mm0  
         punpcklwd mm4, mm6
        punpckhwd mm1, mm6
         por mm2, mm4
        por mm3, mm1
         pmaddwd mm2, mm7     // Multiply pixels by weights.  pixel = img1*weight + img2*invweight (twice)
        pmaddwd mm3, mm7      // Stalls 1 cycle (multiply unit stall)
         paddd mm2, mm5       // Add rounder
        paddd mm3, mm5
         psrld mm2, 15        // Shift down, so there is no fraction.
        psrld mm3, 15        
        packssdw mm2, mm3
        packuswb mm2, mm6 
        movd [esi + eax], mm2

        add eax, 4
        cmp ebx, eax
        jg xloop

        add esi, [dst.pitch];
        add edi, [src.pitch];
        dec ecx
        jnz yloop

        emms
      }
/////////////////////
// Blends two planes.
// A weight between the two planes are given.
// Has rather ok pairing, 
// and has very little memory usage.
// Processes four pixels per loop, so rowsize must be mod 4.
// Thanks to ARDA for squeezing out a bit more performance.
// (c) 2002 by sh0dan.
/////////
    }
}




BufferWindow BufferWindow::FlipVertical() const
{
  BufferWindow result(dimension_, GetEnvironment());

  CWindowPtr src = Read();

  src.to(0, src.height - 1 );   //move to last line
  src.pitch = -src.pitch;       //negate pitch (so it will blit bottom to top)           

  Blitter::Get()( src, result.Write(), dimension_);

  return result;
}


}//namespace avs
