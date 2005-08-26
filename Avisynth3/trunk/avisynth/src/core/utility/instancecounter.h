// Avisynth v3.0 alpha.  Copyright 2003-2005 Ben Rudiak-Gould et al.
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


#ifndef __AVS_UTILITY_INSTANCECOUNTER_H__
#define __AVS_UTILITY_INSTANCECOUNTER_H__

//boost include
#include <boost/thread/mutex.hpp>


namespace avs { namespace utility {



//////////////////////////////////////////////////////////////////////////////////////
//  instance_counter<Tag>
//
//  helper class meant to help plugins implementing the PluginCanUnloadNow export
//
//  it can be achieved using the following code :
//
//  struct MyPluginTag { };
//  typedef avs::instance_counter<MyPluginTag> InstanceCounter;
//
//  class MyObjectExportedToAvs (filters and operations functors qualify)
//  {
//    InstanceCounter counter_;   //add a InstanceCounter member
//    //...
//  };
//
//  bool PluginCanUnloadNow() { return InstanceCounter::Count() == 0; }
//
template <typename Tag> class instance_counter
{

  typedef boost::mutex Mutex;
  typedef boost::mutex::scoped_lock Lock;

  static Mutex mutex_;            //provides synchronisation
  static unsigned long count_;    //instance count


public:  //structors

  instance_counter() { IncreaseCount(); }
  instance_counter(instance_counter<Tag> const&) { IncreaseCount(); }

  ~instance_counter() { DecreaseCount(); }


public:

  static unsigned long Count()
  {
    Lock lock(mutex_);
    return count_;
  }  
  

private:  //implementation

  static void IncreaseCount()
  {
    Lock lock(mutex_);
    ++count_;
  }

  static void DecreaseCount()
  {
    Lock lock(mutex_);
    --count_;
  }

};


template <typename Tag> boost::mutex instance_counter<Tag>::mutex_;
template <typename Tag> int instance_counter<Tag>::count_ = 0;



} } //namespace avs::utility

#endif //__AVS_UTILITY_INSTANCECOUNTER_H__
