// Avisynth v3.0 alpha.  Copyright 2003 David Pierre - Ben Rudiak-Gould et al.
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
#include "concrete.h"
#include "../blitter.h"
#include "../colorspace.h"
#include "../timing/threadclock.h"
#include "../runtime_environment.h"


namespace avs {



OwnedBlock RuntimeEnvironment::NewOwnedBlock(int size, bool recycle)
{
  return OwnedBlock( shared_from_this(), size, recycle );
}


CPVideoFrame RuntimeEnvironment::CreateFrame(ColorSpace& space, Dimension const& dim, FrameType type)
{
  return space.CreateFrame( shared_from_this(), dim, type );
}


Blitter const& RuntimeEnvironment::GetBlitter()
{
  return Blitter::Get();
}


PEnvironment RuntimeEnvironment::Create(int memMax)
{
  typedef environment::concrete<timing::ThreadClock> Environment;

  return PEnvironment( (RuntimeEnvironment *)new Environment(memMax) );
}



} //namespace avs
