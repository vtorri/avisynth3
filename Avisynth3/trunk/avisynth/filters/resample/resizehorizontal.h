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


#ifndef __AVS_FILTERS_RESIZEHORIZONTAL_H__
#define __AVS_FILTERS_RESIZEHORIZONTAL_H__


//avisynth include
#include "resize.h"


#pragma warning ( push )           //push warning state
#pragma warning (disable : 4250)   //get rid of MI dominance decisions


namespace avs { 
  
//class declaration
class BufferWindow;


namespace filters {



class Resize::Horizontal : public Resize
{

  int width_;


public:  //constructor

  Horizontal(PClip child, PFilter filter, int width, const SubRange& subrange);


public:  //colorspace specific inner subclasses

  class RGB24;
  class RGB32;

  class YV12;
  class YUY2;


protected:  //helper method

  static void ProcessPlane(const BufferWindow& src, BufferWindow& dst, const ResamplingPattern::MMXInterleave& pattern);

};



class Resize::Horizontal::RGB24 : public Resize::Horizontal
{

  ResamplingPattern::Straight pattern;


public:  //constructor

  RGB24(PClip child, PFilter filter, int width, const SubRange& subrange)
    : Horizontal( child, filter, width, subrange )
    , pattern( *filter, subrange, width, child->GetVideoInfo().GetWidth() ) { }


private:  //MakeFrame method

  virtual CPVideoFrame MakeFrame(int n) const;

};


class Resize::Horizontal::RGB32 : public Resize::Horizontal
{

  ResamplingPattern::Straight pattern;


public:  //constructor

  RGB32(PClip child, PFilter filter, int width, const SubRange& subrange)
    : Horizontal( child, filter, width, subrange )
    , pattern( *filter, subrange, width, child->GetVideoInfo().GetWidth() ) { }


private:  //MakeFrame method

  virtual CPVideoFrame MakeFrame(int n) const;

};


class Resize::Horizontal::YV12 : public Resize::Horizontal
{

  ResamplingPattern::MMXInterleave luma;
  ResamplingPattern::MMXInterleave chroma;


public:  //constructor

  YV12(PClip child, PFilter filter, int width, const SubRange& subrange);


private:  //MakeFrame method

  virtual CPVideoFrame MakeFrame(int n) const;

};


}; }; //namespace avs::filters

#pragma warning ( pop )

#endif //__AVS_FILTERS_RESIZEHORIZONTAL_H__