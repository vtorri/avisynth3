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


//avisynth includes
#include "avisource.h"
#include "../../core/videoinfo.h"
#include "avi/framedecompressor.h"


namespace avs { namespace filters {


AviSource::AviSource(PEnvironment const& env)
  : clip::caching::Concrete( env )
  , lastIndex_( -1 ) { }


CPVideoFrame AviSource::MakeFrame(int n) const
{
  GetVideoInfo()->CheckHasFrame(n);                  //defends against illegal values

  int startIndex = PreviousKeyFrame(n);              //find the last keyframe

  if ( lastIndex_ < n && startIndex <= lastIndex_ )  //maybe we don't need to go back that far
    startIndex = lastIndex_ + 1;

  CPVideoFrame result;

  for ( int i = startIndex; i <= n; ++i, lastIndex_ = i ) 
  {
    CPVideoFrame frame = (*frameDecompressor_)(i, i != n);

    if ( frame )                //if frame not NULL, aka a valid frame
      result = frame;           //keep it
  }

  return result ? result
                : MakeFrame(startIndex - 1);         //all we decoded was dropped frames, so we take the previous 
}



} } //namespace avs::filters
