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


//avisynth includes
#include "map.h"


namespace avs { namespace cspace { namespace impl {



Map::Map()
{
  internalMap_["RGB24"] = &rgb24_;
  internalMap_["RGB32"] = &rgb32_;
  internalMap_["RGB45"] = &rgb45_;
  internalMap_["YUY2"] = &yuy2_;
  internalMap_["YV12"] = &yv12_;
  internalMap_["YV24"] = &yv24_;
  internalMap_["YV45"] = &yv45_;
}



PColorSpace Map::operator[](std::string const& name) const
{
  Lock lock(mutex_);
  //1st: search internal colorspace map
  InternalColorSpaceMap::const_iterator iit = internalMap_.find(name);  
  if ( iit != internalMap_.end() )        //if found inside
    return (*iit->second)();              //makes the InternalCreator do its work

  PColorSpace result;

  //2nd: search external colorspace map
  ExternalColorSpaceMap::iterator eit = externalMap_.find(name);
  if ( eit != externalMap_.end() )        //if in the map
    if ( ! (result = eit->second()) )     //if creator fetchs an invalid value (empty)
      externalMap_.erase(eit);            //erase obsolete map entry

  return result;                          //return result
}



Map Map::instance;


} } } //namespace avs::cspace::impl

