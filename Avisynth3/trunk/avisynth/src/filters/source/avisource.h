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


#ifndef __AVS_FILTERS_AVISOURCE_H__
#define __AVS_FILTERS_AVISOURCE_H__

//avisynth includes
#include "../../clip/nochild.h"
#include "../../clip/caching/concrete.h"
#include "avi/framedecompressor.h"


namespace avs { namespace filters {


//declaration
namespace avisource { class FrameDecompressor; }



///////////////////////////////////////////////////////////////////////////////////////
//  AviSource
//
//  base class for AviFileSource and OpenDmlSource (unimplemented yet)
//
class AviSource : public clip::NoChild
                , public clip::caching::Concrete
{

  boost::scoped_ptr<avisource::FrameDecompressor> frameDecompressor_;

  mutable int lastIndex_;
  

public:  //structors

  AviSource(PEnvironment const& env);

  //generated destructor is fine


private:  //MakeFrame method

  virtual CPVideoFrame MakeFrame(int n) const;


private:  //AviSource private interface

  //returns index of previous keyframe
  virtual int PreviousKeyFrame(int n) const = 0;

  //reads video data for frame n into passed buffer and returns read data size
  //can be passed a NULL buffer to get the necessary buffer size
  virtual long ReadVideo(int n, BYTE * buffer, long bufferSize) const = 0;

  friend class avisource::FrameDecompressor;   //so it can call ReadVideo


protected:  //protected interface

  void SetFrameDecompressor(avisource::FrameDecompressor * frameDecompressor) { frameDecompressor_.reset( frameDecompressor ); }

};


} } //namespace avs::filters

#endif //__AVS_FILTERS_AVISOURCE_H__
