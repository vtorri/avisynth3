// Avisynth v3.0 alpha.  Copyright 2004 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_COLORSPACE_IMPL_PLANAR_YUV_H__
#define __AVS_COLORSPACE_IMPL_PLANAR_YUV_H__

//avisynh includes
#include "../planar.h"
#include "../../videoframe/planar_yuv.h"
#include "../../exception/nosuchplane.h"
#include "../../exception/colorspace/unsupported.h"
#include "../../exception/colorspace/invalidwidth.h"
#include "../../exception/colorspace/invalidheight.h"


namespace avs { namespace cspace { namespace impl {



template <typename VFrame>
class planar_yuv : public planar::YUV
{

public:  //ColorSpace interface

  virtual bool HasPlane(Plane plane) const
  {
    return plane == PLANAR_Y || plane == PLANAR_U || plane == PLANAR_V;
  }

  virtual PVideoFrame CreateFrame(PEnvironment const& env, Dimension const& dim, FrameType type) const
  {
    return CPVideoFrame( static_cast<VideoFrame *>(new VFrame(dim, type, env)) );
  }


public:  //planar::YUV interface

  virtual PVideoFrame CreateFrame(Dimension const& dim, FrameType type, BufferWindow const& y , BufferWindow const& u, BufferWindow const& v) const
  {
    return CPVideoFrame( static_cast<VideoFrame *>(new VFrame(dim, type, y, u, v)) );
  }

};



/////////////////////////////////////////////////////////////////////////////////
//  YV12
//
//  YV12 ColorSpace subclass
//
class YV12 : public planar_yuv<vframe::YV12>
{

public:  //ColorSpace interface

  virtual ID id() const { return I_YV12; }
  virtual char const * GetName() const { return "YV12"; }
  virtual unsigned long GetFourCC() const { return MakeFourCC('Y', 'V', '1', '2'); }
  virtual int GetBitsPerPixel() const { return 12; }

  virtual int GetBitmapSize(Dimension const& dim) const
  {
    return 3 * RoundUp<4>(dim.GetWidth()) * dim.GetHeight() / 2;
  }

  virtual bool HasProperty(Property prop) const
  {
    return prop == P_PLANAR || prop == P_YUV || prop == P_DEPTH8;
  }

  virtual void Check(int x, int y, bool interlaced = false) const
  {
    if ( x & 1 )                                            //x must be mod 2
      throw exception::cspace::InvalidWidth(shared_from_this(), x, 2);
    if ( interlaced && (y & 3) )                            //y must be mod 4 when interlaced             
      throw exception::cspace::InvalidHeight(shared_from_this(), y, 4, true);
    if ( y & 1 )                                            //y must be mod 2 when not interlaced
      throw exception::cspace::InvalidHeight(shared_from_this(), y, 2, false);
  }

  virtual void ToPlane(int& x, int& y, Plane plane) const
  {
    switch(plane)
    {
    case PLANAR_Y: break;
    case PLANAR_U:
    case PLANAR_V: x >>= 1; y >>= 1; break;

    default:
      throw exception::NoSuchPlane(shared_from_this(), plane);
    }
  }

};




/////////////////////////////////////////////////////////////////////////////////
//  YV24
//
//  YV24 ColorSpace subclass
//
class YV24 : public planar_yuv<vframe::YV24>
{

public:  //ColorSpace interface

  virtual ID id() const { return I_YV24; }
  virtual char const * GetName() const { return "YV24"; }
  virtual unsigned long GetFourCC() const { throw exception::cspace::Unsupported(shared_from_this()); }
  virtual int GetBitsPerPixel() const { return 24; }

  virtual int GetBitmapSize(Dimension const& dim) const
  {
    return 3 * RoundUp<4>(dim.GetWidth()) * dim.GetHeight();
  }

  virtual bool HasProperty(Property prop) const
  {
    return prop == P_PLANAR || prop == P_YUV || prop == P_DEPTH8;
  }

  virtual void Check(int /*x*/, int y, bool interlaced) const
  {
    if ( interlaced && (y & 1) )
      throw exception::cspace::InvalidHeight(shared_from_this(), y, 2, true);
  }

  virtual void ToPlane(int& /*x*/, int& /*y*/, Plane plane) const
  {
    if ( ! HasPlane(plane) )
      throw exception::NoSuchPlane(shared_from_this(), plane);
  }

};



/////////////////////////////////////////////////////////////////////////////////
//  YV45
//
//  YV45 ColorSpace subclass
//
class YV45 : public planar_yuv<vframe::YV45>
{

public:  //ColorSpace interface

  virtual ID id() const { return I_YV45; }
  virtual char const * GetName() const { return "YV45"; }
  virtual unsigned long GetFourCC() const { throw exception::cspace::Unsupported(shared_from_this()); }
  virtual int GetBitsPerPixel() const { return 45; }

  virtual int GetBitmapSize(Dimension const& dim) const
  {
    return 3 * RoundUp<4>(dim.GetWidth() * 2) * dim.GetHeight();
  }

  virtual bool HasProperty(Property prop) const
  {
    return prop == P_PLANAR || prop == P_YUV || prop == P_DEPTH15;
  }

  virtual void Check(int /*x*/, int y, bool interlaced) const
  {
    if ( interlaced && (y & 1) )
      throw exception::cspace::InvalidHeight(shared_from_this(), y, 2, true);
  }

  virtual void ToPlane(int& x, int& /*y*/, Plane plane) const
  {
    if ( ! HasPlane(plane) )
      throw exception::NoSuchPlane(shared_from_this(), plane);

    x <<= 1;
  }

};



} } } //namespace avs::cspace::impl

#endif //__AVS_COLORSPACE_IMPL_PLANAR_YUV_H__
