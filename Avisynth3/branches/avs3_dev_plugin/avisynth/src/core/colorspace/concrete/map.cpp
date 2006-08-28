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


//avisynth includes
#include "map.h"
#include "yuy2.h"
#include "yv12.h"
#include "yv24.h"
#include "rgb24.h"
#include "rgb32.h"
#include "../../exception/colorspace/unknown.h"


namespace avs { namespace colorspace { namespace concrete {



namespace {

template <class CS>
ColorSpace const * Create() { return new CS(); }

} //namespace anonymous


Map::Map()
  : rgb24_( &Create<RGB24> )
  , rgb32_( &Create<RGB32> )
  , yuy2_( &Create<YUY2> )
  , yv12_( &Create<YV12> )
  , yv24_( &Create<YV24> )
{
  nameMap_["RGB24"] = &rgb24_;
  nameMap_["RGB32"] = &rgb32_;
  nameMap_["YUY2"] = &yuy2_;
  nameMap_["YV12"] = &yv12_;
  nameMap_["YV24"] = &yv24_;
}


PColorSpace Map::operator[](std::string const& name) const
{
  Lock lock(mutex_);

  //search name to colorspace map
  NameToColorSpaceMap::const_iterator it = nameMap_.find(name);  
  if ( it != nameMap_.end() )            //if found inside
    return it->second->Get(); 

  throw exception::cspace::Unknown();
}



} } } //namespace avs::colorspace::concrete
