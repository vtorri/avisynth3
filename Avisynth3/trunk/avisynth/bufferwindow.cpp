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

#include "bufferwindow.h"
#include "bitblit.h"

/**********************************************************************************************/
/************************************ VideoFrameBuffer ****************************************/
/**********************************************************************************************/










/*

AlphaForwardingBuffer::AlphaForwardingBuffer(int row_size, int height)
: ForwardingBuffer(row_size, FRAME_ALIGN)
{
  int size = SizeFor(pitch, height);
  if (largest.empty() || largest->GetSize() < size)
    largest = new SharedAlphaBuffer(size);
  
  forwardTo = largest;
}

*/

/**********************************************************************************************/
/************************************** BufferWindow ******************************************/
/**********************************************************************************************/


BufferWithOffset::BufferWithOffset(BufferWithOffset& other, PEnvironment env)
  : vfb(other.vfb), offset(other.offset)
{
  if ( env != GetEnvironment() )   //if same env, nothing more to do
  { 
    if (! other.vfb->IsEnvSharingEnabled() )
      other.vfb = new ForwardToSharingBuffer(other.vfb);   //make sure sharing is enabled
    vfb = other.vfb->ShareWith(env);                       //change vfb as a shared frame buffer
  }
}

void BufferWithOffset::EnsureWritable(const Dimension& dimension)
{
  if ( ! vfb->IsWriteAllowed() )
  {
    BufferWithOffset temp(dimension, GetEnvironment());
    Blitter::Blit(temp.GetWritePtr(), temp.GetPitch(), GetReadPtr(), GetPitch(), dimension);
    *this = temp;
  }
  return *this;
}





void BufferWindow::Copy(const BufferWindow& temp, const Vecteur& coords)
{
  BufferWindow other = temp;   //trick to gracefully handle the other == this case

  Vecteur topLeft = LimitToWindow(coords);
  Vecteur bottomRight = LimitToWindow(coords + other.dimension);

  Dimension toCopy = bottomRight - topLeft;

  if (! toCopy.empty() )
    Blitter::Blit(
      GetWritePtr(topLeft), GetPitch(),
      other.GetReadPtr(topLeft - coords), other.GetPitch(),
      toCopy
    );                                         
}

void BufferWindow::SizeChange(const Vecteur& topLeft, const Vecteur& bottomRight)
{
  BufferWindow saved = *this;   //save actual position
  //updating parameters
  dimension +=  bottomRight - topLeft;  
  offset += VecteurToOffset(topLeft);
  //three conditions where needed to reallocate the Buffer
  int pitch = GetPitch();
  if ( GetWidth() > pitch || offset < 0 || offset + GetHeight() * pitch > vfb->GetSize() )
  {
    BufferWindow resized(dimension, GetEnvironment());  //new buffer
    resized.Copy(saved, -topLeft);    //copy saved data at the right place
    *this = resized;                  //replace self by resized                      
  }
}

void BufferWindow::Blend(const BufferWindow& other, float factor)
{  
  if (this != &other && factor > 0) //otherwise nothing to do
    if (factor >= 1)                //full blend of other into self
      *this = other;                //we replace this window by the other one
    else {
      //TODO: Code ME !!
    }
}

void BufferWindow::FlipVertical()
{
  BufferWindow result(dimension, GetEnvironment());

  Blitter::Blit(result.GetWritePtr(), result.GetPitch(), 
                GetReadPtr() + (GetHeight() - 1) * GetPitch(), -GetPitch(), dimension);
  *this = result;
}


