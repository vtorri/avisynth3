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


#ifndef __AVS_DIRECTSTREAM_DIRECTSTREAMER_H__
#define __AVS_DIRECTSTREAM_DIRECTSTREAMER_H__

//avisynth includes
#include "../define.h"
#include "../core/forward.h"
#include "../core/property.h"

//boost include
#include <boost/weak_ptr.hpp>


namespace avs { namespace directstream {



////////////////////////////////////////////////////////////////////////////////////
//  DirectStreamer
//
//  Property attached to frames valid for direct streaming,
//  which provides the methods to get direct stream data
//
//  It is meant to be subclassed for each kind of directstreamable source.
//  The methods defined at this level are only for directstream legality analysis
//
//
//  NB: subclass implementers, be wary of circular references
//      these should only have weak_ptr back ref to the source clip
//
class AVS_NOVTABLE DirectStreamer : public Property
{

  WeakPClip source_;
  int n_;


public:  //structors

  DirectStreamer(PClip const& source, int n)
    : source_( source )
    , n_( n ) { }

  //generated copy constructor and destructor are fine


public:  //Property interface

  virtual bool IsStatic() const { return true; }


public:  //DirectStreamer public interface

  virtual bool CanStartHere() const = 0;    //report if a segment can start with the frame (ie keyframe)
  virtual bool CanStopHere() const = 0;     //report if a segment can stop with the frame

  bool CanFollow(DirectStreamer const& other) const
  {
    //try the most common case first : ie two frames from same clip
    if ( ! (source_ < other.source_) && ! (other.source_ < source_)    //ugly for source == other.source_
       && (  n_ == other.n_ + 1                          //and frames are consecutives
          || ( CanStartHere() && other.CanStopHere() )   //or possible to cut on them
          )
       )
      return true;                  //success

    //else need to check video compatibilty and still need to cut there
    return IsVideoCompatible(other) && ( CanStartHere() && other.CanStopHere() );
  }


private:  //DirectStreamer private interface

  virtual bool IsVideoCompatible(DirectStreamer const& other) const = 0;


protected:  //read access

  PClip GetSource() const { return PClip(source_); }
  int GetN() const { return n_; }

};


} } //namespace avs::directstream

#endif //__AVS_DIRECTSTREAM_DIRECTSTREAMER_H__
