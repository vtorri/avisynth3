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


#ifndef __AVS_CS_YUY2_H__
#define __AVS_CS_YUY2_H__

//avisynh include
#include "interleaved.h"
#include "../videoframe/yuy2.h"


namespace avs { namespace cspace {



/////////////////////////////////////////////////////////////////////////////////////////
//  YUY2
//
//  YUY2 ColorSpace subclass
//
class YUY2 : public interleaved<2, true>
{

private:  //constructor

  YUY2() { }


public:  //ColorSpace interface

  virtual ID id() const { return I_YUY2; }
  virtual char const * GetName() const { return "YUY2"; }

  virtual bool HasProperty(Property prop) const
  { 
    return prop == YUV || prop == INTERLEAVED || prop == DEPTH8;
  }

  virtual void Check(int x, int y, bool interlaced = false) const 
  {
    interleaved<2, true>::Check(x, y, interlaced);
    if ( x & 1 )                                      //if x is not even
      ThrowInvalidWidthException(2, x);               //exception
  }

  virtual CPVideoFrame CreateFrame(PEnvironment const& env, Dimension const& dim, FrameType type) const
  {
    return CPVideoFrame( (VideoFrame *)new vframe::YUY2(dim, type, env) );
  }


public: 

  static YUY2 instance;

};


} } //namespace avs::cspace

#endif //__AVS_CS_YUY2_H__
