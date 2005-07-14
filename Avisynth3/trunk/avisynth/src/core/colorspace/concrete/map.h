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


#ifndef __AVS_COLORSPACE_CONCRETE_MAP_H__
#define __AVS_COLORSPACE_CONCRETE_MAP_H__

//avisynth includes
#include "../../forward.h"               //for PColorSpace
#include "../../utility/valuecache.h"

//stl includes
#include <map>
#include <string>

//boost include
#include <boost/thread/mutex.hpp>


namespace avs { namespace colorspace { namespace concrete {



//////////////////////////////////////////////////////////////////////////////////
//  cspace::Map
//
//  map used by the ColorSpace::FromString method to get ColorSpaces from names
//
class Map
{
  
  typedef boost::mutex Mutex;
  typedef Mutex::scoped_lock Lock;
  typedef utility::value_cache<ColorSpace const> ColorSpaceCache;
  typedef std::map<std::string, ColorSpaceCache *> NameToColorSpaceMap;

  mutable Mutex mutex_;
  
  ColorSpaceCache rgb24_, rgb32_, yuy2_, yv12_, yv24_;

  NameToColorSpaceMap nameMap_;


public:  //structors

  Map();

  //generated destructor is fine


public:  //Map interface
  
  //map like read access
  PColorSpace operator[](std::string const& name) const;

  PColorSpace rgb24() const { Lock lock(mutex_); return rgb24_.Get(); }
  PColorSpace rgb32() const { Lock lock(mutex_); return rgb32_.Get(); }
  PColorSpace yuy2() const { Lock lock(mutex_); return yuy2_.Get(); }
  PColorSpace yv12() const { Lock lock(mutex_); return yv12_.Get(); }
  PColorSpace yv24() const { Lock lock(mutex_); return yv24_.Get(); }

};



} } } //namespace avs::colorspace::concrete

#endif //__AVS_COLORSPACE_CONCRETE_MAP_H__
