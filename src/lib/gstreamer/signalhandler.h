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


#ifndef __AVS_GSTREAMER_SIGNALHANDLER_H__
#define __AVS_GSTREAMER_SIGNALHANDLER_H__

//avs include
#include "forward.h"                 //for Object declaration

//boost include
#include <boost/noncopyable.hpp>



namespace avs { namespace gstreamer {



///////////////////////////////////////////////////////////////////////////////////////////
//  SignalHandler
//
//  RAII wrapper that makes sure connected signals are disconnected
//
class SignalHandler : public boost::noncopyable
{

  Object * target_;
  unsigned long signalId_;

  static unsigned long Connect(Object * target, char const * signalName, void (*callBack)(), void * data);


public:  //structors

  template <typename CallBack>
  SignalHandler(Object * target, char const * signalName, CallBack callBack, void * data)
    : target_( target )
    , signalId_( Connect(target, signalName, reinterpret_cast<void (*)()>(callBack), data) ) { }
  
  ~SignalHandler();


public:  //access

  Object * GetTarget() const { return target_; }

};



} } //namespace avs::gstreamer

#endif //__AVS_GSTREAMER_SIGNALHANDLER_H__
