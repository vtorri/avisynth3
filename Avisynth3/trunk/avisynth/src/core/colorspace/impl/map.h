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


#ifndef __AVS_COLORSPACE_IMPL_MAP_H__
#define __AVS_COLORSPACE_IMPL_MAP_H__

#ifdef _MSC_VER
#pragma warning ( push )           //push warning state
#pragma warning (disable : 4275)   //non dll-interface class 'boost::noncopyable' used as base for dll-interface class 'boost::mutex'
#endif 

//avisynth includes
#include "../../forward.h"      //for PColorSpace
#include "externalcreator.h"
#include "internalcreator.h"

//stl includes
#include <map>
#include <string>

//boost include
#include <boost/thread/mutex.hpp>


namespace avs { namespace cspace { namespace impl {



//////////////////////////////////////////////////////////////////////////////////
//  cspace::Map
//
//  map used by the ColorSpace::FromString method to get ColorSpaces from names
//
class Map
{
  
  typedef boost::mutex Mutex;
  typedef Mutex::scoped_lock Lock;
  typedef std::map<std::string, InternalCreator *> InternalColorSpaceMap;
  typedef std::map<std::string, ExternalCreator> ExternalColorSpaceMap;

  mutable Mutex mutex_;
  
  InternalCreator rgb24_, rgb32_, rgb45_, yuy2_, yv12_, yv24_, yv45_;

  InternalColorSpaceMap internalMap_;
  mutable ExternalColorSpaceMap externalMap_;


private:  //structors

  Map();

  //generated destructor is fine


public:  //Map interface
  
  //map like read access
  PColorSpace operator[](std::string const& name) const;

  //adds a (external) colorspace to the map
  void Register(PColorSpace const& space);

  PColorSpace rgb24() const { Lock lock(mutex_); return rgb24_(); }
  PColorSpace rgb32() const { Lock lock(mutex_); return rgb32_(); }
  PColorSpace rgb45() const { Lock lock(mutex_); return rgb45_(); }
  PColorSpace yuy2() const { Lock lock(mutex_); return yuy2_(); }
  PColorSpace yv12() const { Lock lock(mutex_); return yv12_(); }
  PColorSpace yv24() const { Lock lock(mutex_); return yv24_(); }
  PColorSpace yv45() const { Lock lock(mutex_); return yv45_(); }


public:  //sole instance

  static Map instance;

};



} } } //namespace avs::cspace::impl

#ifdef _MSC_VER
#pragma warning ( pop )
#endif 

#endif //__AVS_COLORSPACE_IMPL_MAP_H__
