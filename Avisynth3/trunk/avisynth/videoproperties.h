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


#ifndef __VIDEOPROPERTIES_H__
#define __VIDEOPROPERTIES_H__


#include "refcounted.h"
#include "colorspace.h"  //which includes geometric.h
#include "boost/rational.hpp"  //for rational

typedef boost::rational<unsigned> Fraction;  //type used to hold fps


//commmon superclass for ClipVideoProperties (holds video info in VideoInfo)
//and FrameVideoProperties (does the same in VideoFrame)
class VideoProperties : public RefCounted {

  const ColorSpace * space;
  Dimension dimension;

protected:
  static void Check(const ColorSpace& space, const Dimension& dimension, bool MaybeInterlaced) { space.IsLegalCoords(dimension, MaybeInterlaced); }  

  //default implementation: no interlacing possible
  virtual bool MaybeInterlaced() const { return false; }
  
  //may be useful
  bool operator==(const VideoProperties& other) const { return space == other.space && dimension == other.dimension; }

public:
  VideoProperties(const ColorSpace& _space, const Dimension& _dimension)
    : space(&_space), dimension(_dimension) { Check(_space, dimension, false); }

  const ColorSpace& GetColorSpace() const { return *space; }
  const Dimension& GetDimension() const { return dimension; }
  int GetWidth() const { return dimension.GetWidth(); }
  int GetHeight() const { return dimension.GetHeight(); }

  void SetColorSpace(const ColorSpace& _space) { Check(_space, dimension, MaybeInterlaced()); space = &_space; }  
  void SetDimension(const Dimension& dim) { Check(*space, dim, MaybeInterlaced()); dimension = dim; }  
  void SetWidth(int width) { SetDimension(Dimension(width, GetHeight())); }
  void SetHeight(int height) { SetDimension(Dimension(GetWidth(), height)); }

};


//class used to hold all video info
class ClipVideoProperties : public VideoProperties {

  bool frameClip;   //true if frame clip, false if field clip                      
  Fraction fps;
  int frameCount;

public:
  ClipVideoProperties(const ColorSpace& space, const Dimension& dimension)
    : VideoProperties(space, dimension), frameClip(true), fps(25), frameCount(25) { }
  ClipVideoProperties(const ClipVideoProperties& other)
    : VideoProperties(other), frameClip(other.frameClip), fps(other.fps), frameCount(other.frameCount) { }

  bool IsFrameClip() const { return frameClip; }
  const Fraction& GetFPS() const { return fps; }
  int GetFrameCount() const { return frameCount; }

  void SetFrameClip(bool _frameClip) { frameClip = _frameClip; }
  void SetFPS(const Fraction& _fps) { fps = _fps; }
  void SetFrameCount(int _frameCount)
  {
    if (frameCount <= 0)
      throw invalid_argument("FrameCount should be strictly positive");
  }

  ClipVideoProperties * clone() const { return new ClipVideoProperties(*this); }
};

typedef smart_ptr<ClipVideoProperties> PClipVidProps;


enum FrameType {
  FIELD_TOP,
  FIELD_BOTTOM,
  PROGRESSIVE,
  INTERLACED_TFF,
  INTERLACED_BFF,
  UNKNOWN
};




//class holding video properties for VideoFrame (so can be shared)
class FrameVideoProperties : public VideoProperties {

  FrameType frameType;

  static bool MaybeInterlaced(FrameType frameType) { return frameType > PROGRESSIVE; }

protected:
  virtual bool MaybeInterlaced() const { return MaybeInterlaced(frameType); } 

public:
  FrameVideoProperties(const ClipVideoProperties& cvp) : VideoProperties(cvp)
  {
    if (! cvp.IsFrameClip() )
      frameType = FIELD_TOP;
    else {
      try { 
        Check(GetColorSpace(), GetDimension(), true);
        frameType = UNKNOWN;   //since it can be interlaced, it is unknown
      }
      catch(invalid_argument&) { frameType = PROGRESSIVE; }  //can't be interlaced so progressive
    }
  } 
  FrameVideoProperties(const FrameVideoProperties& other) : VideoProperties(other), frameType(other.frameType) { }

  bool IsField() const { return frameType < PROGRESSIVE; }
  bool IsFrame() const { return frameType >= PROGRESSIVE; }

  FrameType GetFrameType() const { return frameType; }

  void SetFrameType(FrameType _frameType)
  { 
    Check(GetColorSpace(), GetDimension(), MaybeInterlaced(_frameType));
    frameType = _frameType;
  }


  Dimension GetPlaneDimension(Plane plane) const
  { 
    Vecteur v = (GetColorSpace().ToPlaneCoords(GetDimension(), plane));
    return Dimension(v.GetX(), v.GetY());
  }

  FrameVideoProperties * clone() const { return new FrameVideoProperties(*this); }
};

typedef smart_ptr<FrameVideoProperties> PFrameVidProps;



#endif  //#define __VIDEOPROPERTIES_H__
