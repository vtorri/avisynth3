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


#ifndef __AVS_COLORSPACE_CONCRETE_YV24_H__
#define __AVS_COLORSPACE_CONCRETE_YV24_H__

//avisynth includes
#include "../yuv/depth8.h"
#include "../yuv/planar.h"


namespace avs { namespace colorspace { namespace concrete {



/////////////////////////////////////////////////////////////////////////////////////////////
//  YV24
//
//
class YV24 : public yuv::Planar
           , public yuv::Depth8
{

public:  //ColorSpace interface

  virtual ID id() const { return I_YV24; }
  virtual char const * GetName() const { return "YV24"; }

  virtual void Check(long x, long y, bool interlaced = false) const;
  virtual void ToPlane(long& x, long& y, char plane) const;


public:  //blank frame creation method

  virtual PVideoFrame CreateFrame(PEnvironment const& env, Dimension const& dim, FrameType type) const;


public:  //fetch exporter(s)

  virtual PExporter GetExporter(std::string const& type) const;


public:  //yuv::Planar interface

  virtual PVideoFrame CreateFrame(Dimension const& dim, FrameType type, BufferWindow const& y , BufferWindow const& u, BufferWindow const& v) const;

};



} } } //namespace avs::colorspace::concrete

#endif //__AVS_COLORSPACE_CONCRETE_YV24_H__
