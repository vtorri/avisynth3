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
#include "../core/property.h"



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

public:  //Property interface

  virtual bool IsStatic() const { return true; }


public:  //DirectStreamer interface

  virtual bool CanStartHere() const = 0;
  virtual bool CanStopHere() const = 0;

  virtual bool CanFollow(DirectStreamer const& other) const = 0;

};


} } //namespace avs::directstream

#endif //__AVS_DIRECTSTREAM_DIRECTSTREAMER_H__
