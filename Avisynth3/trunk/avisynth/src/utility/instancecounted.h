// Avisynth v3.0 alpha.  Copyright 2004 Ben Rudiak-Gould et al.
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


#ifndef __AVS_INSTANCECOUNTED_H__
#define __AVS_INSTANCECOUNTED_H__

#ifdef _MSC_VER
#pragma warning ( push )           //push warning state
#pragma warning (disable : 4275)   //non dll-interface class 'boost::noncopyable' used as base for dll-interface class 'boost::mutex'
#endif 

//boost include
#include <boost/thread/mutex.hpp>


namespace avs {



//////////////////////////////////////////////////////////////////////////////////////
//  instance_counted<Tag>
//
//  helper class meant to help plugins implementing the PluginCanUnloadNow export
//
//  it can be achieved using the following code :
//
//  struct MyPluginTag { };
//  typedef avs::instance_counted<MyPluginTag> InstanceCounted;
//
//  class MyObjectExportedToAvs (filters and operations functors qualify)
//  {
//    InstanceCounted instCounted_;   //add a InstanceCounted member
//    //...
//  };
//
//  bool PluginCanUnloadNow() { return InstanceCounted::Count() == 0; }
//
template <typename Tag> class instance_counted
{

  typedef boost::mutex Mutex;
  typedef boost::mutex::scoped_lock Lock;

  static Mutex mutex_;            //provides synchronisation
  static int count_;              //instance count


public:  //structors

  instance_counted() { IncreaseCount(); }
  instance_counted(instance_counted<Tag> const&) { IncreaseCount(); }

  ~instance_counted() { DecreaseCount(); }


public:

  static int Count()
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


template <typename Tag> boost::mutex instance_counted<Tag>::mutex_;
template <typename Tag> int instance_counted<Tag>::count_ = 0;



} //namespace avs

#ifdef _MSC_VER
#pragma warning ( pop )
#endif 

#endif //__AVS_INSTANCECOUNTED_H__
