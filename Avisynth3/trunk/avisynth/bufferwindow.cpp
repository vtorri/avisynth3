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
/************************************ VideoFrameBuffer  ****************************************/
/**********************************************************************************************/






void EnvSharingBuffer::RemoveSharingEnv(PEnvironment env)
{
  if ( env != GetEnvironment() )                                          //if not current owner
    sharingEnvs.erase(find(sharingEnvs.begin(), sharingEnvs.end(), env)); //remove from waiting list
  else                                 //current oxner want to release ownership
    if ( ! sharingEnvs.empty() )       //if another is interested
    {
      env = sharingEnvs.back();        //get one 
      sharingEnvs.pop_back();          //remove it from list
      forwardTo->SetEnvironment(env);  //take ownership of buffer
    }
}






/**********************************************************************************************/
/************************************** BufferWindow ******************************************/
/**********************************************************************************************/


void BufferWindow::MakeBufferWritable() const
{
  if ( ! vfb->IsWritable() )          //if buffer not writable
  {
    int new_offset;                   //create a frame buffer to accomodate data from self
    PVideoFrameBuffer new_vfb = MakeFrameBuffer(dimension, new_offset, GetEnvironment());
    //copy the relevant data into it
    Blitter::Blit(new_vfb->GetWritePtr() + new_offset, new_vfb->GetPitch(), GetReadPtr(), GetPitch(), dimension);
    vfb = new_vfb;
    offset = new_offset;              //update frame buffer and offset
  }
}


BufferWindow::BufferWindow(const BufferWindow& other, PEnvironment env)
 : dimension(other.dimension), vfb(other.vfb)
{
  if ( env != GetEnvironment() )                         //if env is not already the current owner
  {
    other.MakeBufferWritable();                          //make sure other owns its frame buffer

    if (! other.vfb->IsEnvSharingEnabled() )
      other.vfb = new ForwardToSharingBuffer(other.vfb); //make sure sharing is enabled
    vfb = other.vfb->ShareWith(env);                     //change vfb as a shared frame buffer
  }
  offset = other.offset;                                 //update offset (done here coz MakeBufferWritable can change it)
}


void BufferWindow::Copy(const BufferWindow& temp, const Vecteur& coords)
{
  BufferWindow other = temp;   //trick to gracefully handle the other == this case

  Vecteur topLeft = LimitToWindow(coords);
  Vecteur bottomRight = LimitToWindow(coords + other.dimension);

  Dimension toCopy = Dimension(bottomRight - topLeft);

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
    vfb = MakeFrameBuffer(dimension, offset, GetEnvironment()); //new frame buffer
    Copy(saved, -topLeft);                    //copy saved data at the right place                      
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


