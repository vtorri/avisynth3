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


#ifndef __AVS_TIMING_TIMER_H__
#define __AVS_TIMING_TIMER_H__


//avisynth include
#include "chrono.h"

//boost include
#include <boost/utility.hpp>   //for noncopyable


namespace avs { namespace timing {


//declaration
template <class Timer> class timerstack;



//////////////////////////////////////////////////////////////////////////////////////////////////////
//  timer<Clock>
//
//
//
template <class Clock> class timer : boost::noncopyable
{

  int count_;             //number of times it has run
  __int64 start_;         //time of last start
  __int64 elapsed_;       //elapsed time


public:  //structors

  timer()
    : count_( 0 )
    , elapsed_( 0 ) { }

  //default destructor is fine


public:

  __int64 elapsed() const { return elapsed_; }
  int count() const { return count_; }
  double average() const { return double(elapsed_) / count_; }

  void reset() { elapsed_ = 0; count_ = 0; }


private:

  void start() { ++ count_; restart(); }
  void stop() { elapsed_ += Clock()() - start_;  }

  void interrupt() { stop(); }
  void restart() { start_ = Clock()(); }


public:

  typedef chrono<timer<Clock> > Chrono;

  friend class chrono<timer<Clock> >;
  friend class timerstack<timer<Clock> >;

};






} } //namespace avs::timing

#endif //__AVS_TIMING_TIMER_H__