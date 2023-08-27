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


#ifndef __AVS_ENVIRONMENT_CONCRETE_H__
#define __AVS_ENVIRONMENT_CONCRETE_H__

//avisynth includes
#include "base.h"
#include "../timing/timer.h"
#include "../cache/concrete.h"
#include "../cache/cachelogic.h"
#include "../timing/timerstack.h"


namespace avs { namespace environment {



////////////////////////////////////////////////////////////////////////////////////
//  Concrete
//
//  implementation of the RuntimeEnvironment interface
//
template <class Clock> class concrete : public Base
{

public:  //typedefs

  typedef concrete<Clock> EnvType;
  typedef timing::timer<Clock> Timer;
  typedef timing::timerstack<Timer> TimerStack;


private:  //members

  TimerStack timerStack_;


public:  //structors

  concrete(int memMax)
    : Base( memMax ) { }

  //generated destructor is fine


public:  //NewCache method

  virtual avs::Cache * CreateCache(FrameMaker const& maker)
  {
    typedef cache::concrete<cache::CacheLogic, EnvType> Cache;
    return new Cache( boost::static_pointer_cast<EnvType>(shared_from_this()), maker );
  }

private:  //stop/restart timing when attempting memory cleanup

  virtual void InterruptTiming() { timerStack_.interrupt(); }
  virtual void RestartTiming() { timerStack_.restart(); }


public:  //TimerStackMap struct

  struct TimerStackMap
  {

    typedef timing::timer<Clock> Timer;
    typedef std::shared_ptr<EnvType> Key;

    TimerStack& operator[](Key const& key) const { return key->timerStack_; }

  };

};


} } //namespace avs::environment

#endif //__AVS_RUNTIME_ENVIRONMENT_CONCRETE_H__
