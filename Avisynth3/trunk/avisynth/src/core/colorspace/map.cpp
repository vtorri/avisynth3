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


//avisynth include
#include "map.h"



namespace avs { namespace cspace {


Map::Map()
{
  map_["RGB24"] = &ColorSpace::rgb24();
  map_["RGB32"] = &ColorSpace::rgb32();
  map_["RGB45"] = &ColorSpace::rgb45();
  map_["YUY2"] = &ColorSpace::yuy2();
  map_["YV12"] = &ColorSpace::yv12();
  map_["YV24"] = &ColorSpace::yv24();
  map_["YV45"] = &ColorSpace::yv45();
}


ColorSpace * Map::operator[](std::string const& name) const
{
  ColorSpaceMap::const_iterator it = map_.find(name);
  return it == map_.end() ? NULL
                          : it->second;
}



} } //namespace avs::cspace
