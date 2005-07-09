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


#ifndef __AVS_VIDEOFRAME_CONCRETE_PLANAR_YUV_H__
#define __AVS_VIDEOFRAME_CONCRETE_PLANAR_YUV_H__

//avisynth includes
#include "../base.h"
#include "../../../ownedblock.h"
#include "../../../bufferwindow.h"  //those two so BufferWindow is defined


namespace avs { namespace videoframe { namespace concrete { namespace planar {



//////////////////////////////////////////////////////////////////////////////////////////
//  planar::YUV
//
//
//
class AVS_NOVTABLE YUV : public Base
{

  BufferWindow y_, u_, v_;


public:  //structors

  //normal constructor
  YUV(PColorSpace const& space, Dimension const& dim, FrameType type, PEnvironment const& env);

  //constructs using given buffers
  YUV(PColorSpace const& space, Dimension const& dim, FrameType type, BufferWindow const& y, BufferWindow const& u, BufferWindow const& v);

  //generated copy constructor and destructor are fine


public:  //fetch environment method

  virtual PEnvironment const& GetEnvironment() const { return y_.GetEnvironment(); }


public:  //plane access

  virtual BufferWindow& operator[](Plane plane);
  virtual BufferWindow const& operator[](Plane plane) const;

  BufferWindow& GetY() { ClearStaticProperties(); return y_; }
  BufferWindow& GetU() { ClearStaticProperties(); return u_; }
  BufferWindow& GetV() { ClearStaticProperties(); return v_; }

  BufferWindow const& GetConstY() const { return y_; }
  BufferWindow const& GetConstU() const { return u_; }
  BufferWindow const& GetConstV() const { return v_; }

};



} } } } //namespace avs::videoframe::concrete::planar

#endif //__AVS_VIDEOFRAME_CONCRETE_PLANAR_YUV_H__
