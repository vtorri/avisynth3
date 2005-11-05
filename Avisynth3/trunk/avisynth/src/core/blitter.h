// Avisynth v3.0 alpha.  Copyright 2003-2005 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_BLITTER_H__
#define __AVS_BLITTER_H__

//avisynth include
#include "forward.h"


namespace avs {



/////////////////////////////////////////////////////////////////////////////////////
//  Blitter
//
//  functor interface for Blit operations
//
class Blitter
{

public:  //Blit methods

  virtual BYTE * Blit(CWindowPtr const& src, BYTE * ptr, int pitch) const;
  virtual BYTE const * Blit(BYTE const * src, int pitch, WindowPtr const& dst) const;
  virtual void Blit(CWindowPtr const& src, WindowPtr const& dst, Dimension const& dim) const;

  //others are implemented in terms of this version
  //normally you never directly use this one
  virtual void Blit(BYTE const *& srcp, int srcPitch, BYTE *& dstp, int dstPitch, Dimension const& dim) const = 0;


public:  //access to instance

  static Blitter const& Get();

};



} //namespace avs

#endif //__AVS_BLITTER_H__
