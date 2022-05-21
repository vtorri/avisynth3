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


#ifndef __AVS_VIDEOFRAME_CONCRETE_RGB24_H__
#define __AVS_VIDEOFRAME_CONCRETE_RGB24_H__

//avisynth includes
#include "interleaved.h"
#include "../rgb/depth8.h"


namespace avs { namespace videoframe { namespace concrete {



///////////////////////////////////////////////////////////////////////////////////////
//  RGB24
//
//  RGB24 specific VideoFrame subclass
//
class RGB24 : public Interleaved
            , public rgb::Depth8
{

public:  //structors

  //normal constructor
  RGB24(Dimension const& dim, FrameType type, PEnvironment const& env);

  //constructs using the given buffer
  RGB24(Dimension const& dim, FrameType type, BufferWindow const& main);

  //generated copy constructor and destructor are fine


public:  //clone method

  virtual CPVideoFrame clone() const;


public:  //general frame info

  virtual PColorSpace GetColorSpace() const;


public:  //rgb::Depth8 interface

  virtual void ApplyToR(ByteMap const& r);
  virtual void ApplyToG(ByteMap const& g);
  virtual void ApplyToB(ByteMap const& b);
  virtual void ApplyToA(ByteMap const& a) { }

};




} } } //namespace avs::videoframe::concrete

#endif //__AVS_VIDEOFRAME_CONCRETE_RGB24_H__
