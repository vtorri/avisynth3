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
#include "blitter.h"
#include "vecteur.h"
#include "block/align.h"
#include "bufferwindow.h"


namespace avs {



BufferWindow::BufferWindow(Dimension const& dim, PEnvironment const& env)
  : dim_( dim )
  , pitch_( block::AlignValue(width()) )
  , offset_( block::Align )
  , buffer_( env, pitch() * height() + block::Align * 2, true ) { }


void BufferWindow::swap(BufferWindow& other)
{
  dim_.swap(other.dim_);
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

  Dimension dim(dim_);
  
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
    dim_ = dim;                           //else commit updated parameters
    offset_ = offs;
  }

}



void BufferWindow::Copy(BufferWindow other, Vecteur const& coords)
{
  Vecteur topLeft = coords >> dim_;
  Vecteur bottomRight = ( coords + other.dim_ ) >> dim_;

  Dimension toCopy = Dimension(bottomRight - topLeft);

  if (! toCopy.empty() )
    Blitter::Get()( other.Read() += topLeft - coords, Write() += topLeft, toCopy ); 
}




}//namespace avs
