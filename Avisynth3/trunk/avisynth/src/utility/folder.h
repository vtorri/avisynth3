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


#ifndef __AVS_FOLDER_H__
#define __AVS_FOLDER_H__

#ifdef _MSC_VER
#pragma warning ( push )           //push warning state
#pragma warning (disable : 4275)   //non dll-interface class 'boost::noncopyable' used as base for dll-interface class 'boost::mutex'
#endif 

//boost include
#include <boost/thread/mutex.hpp>

//stl include
#include <map>


namespace avs {



/////////////////////////////////////////////////////////////////////////////////////
//  Folder<Key, Value, Expired>
//
//
//
template <class Key, class Value, class Expired> class folder
{

  typedef folder<Key, Value, Expired> FolderType;
  typedef std::map<Key, Value> InstanceMap;

  typedef boost::mutex Mutex;
  typedef boost::mutex::scoped_lock Lock;

  Mutex mutex_;            //provides synchronisation
  InstanceMap map_;        //underlying map
  unsigned op_count_;      //used to regularly clean up the map of obsolete entries

  Expired const expired;   //functor used to detect obsolete entries


public:  //structors

  folder()
    : op_count_( 0 ) { }

  folder(FolderType const& other)
    : op_count_( 0 )
  {
    Lock(other.mutex_);

    map_ = other.map_;
  }

  //generated destructor is fine


public:  //assignment

  FolderType& operator=(FolderType const& other)
  {
    FolderType tmp(other);
    
    swap(tmp);

    return *this;
  }

  void swap(FolderType& other)
  {
    if ( &other != this )
    {
      //ensure mutex are always locked in same order, to avoid deadlocks
      Lock( &other < this ? mutex_ : other.mutex_ );
      Lock( &other < this ? other.mutex_ : mutex_ );

      map_.swap(other.map_);
      std::swap(op_count_, other.op_count_);
    }
  }


public:  //Proxy inner class

  ///////////////////////////////////////////////////////////////////////////////
  //  Proxy
  //
  //  helps provide map-like access to the underlying map
  //
  class Proxy
  {
    
    FolderType& parent_;
    Key const& key_;


  public:  //constructor

    Proxy(Key const& key, FolderType& parent)
      : parent_( parent )
      , key_( key )
    { 
      parent.TickToCleanUp();
    }


  public:  //Proxy interface

    //associates passed value to its key into underlying map
    //and replace it by folded value if key was already known
    Value operator=(Value const& value)
    {
      Lock lock( parent_.mutex_ );

      typename InstanceMap::iterator it = parent_.map_.find(key_);
      if ( it != parent_.map_.end() )
        return it->second;

      parent_.map_[ key_ ] = value;
      return value;
    }

    //retrieve value associated to key
    operator Value ()
    {
      Lock lock( parent_.mutex_ );

      typename InstanceMap::iterator it = parent_.map_.find( key_ );
      return it != parent_.map_.end() ? it->second : Value();
    }
  
  };


public:  //Folder interface

  Proxy operator[](Key const& key) { return Proxy(key, *this); }


  void TickToCleanUp()
  {
    Lock lock(mutex_);

    if ( ++op_count_ >= 50 )
    {
      UnlockedCleanUp();
      op_count_ = 0;
    }
  }

  void CleanUp()
  {
    Lock lock(mutex_);
    UnlockedCleanUp();
  }


private:  //implementation detail

  void UnlockedCleanUp()
  {
    for(typename InstanceMap::iterator it = map_.begin(); it != map_.end(); )
      if ( expired(it->first) || expired(it->second) )
        map_.erase(it++);
      else ++it;
  }

}; 




} //namespace avs

#ifdef _MSC_VER
#pragma warning ( pop )
#endif 

#endif //__AVS_FOLDER_H__
