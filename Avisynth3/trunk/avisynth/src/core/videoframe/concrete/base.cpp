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
#include "base.h"
#include "../../property.h"
#include "../../colorspace.h"

//boost include
#include <boost/bind.hpp>


namespace avs { namespace vframe { namespace concrete {


Base::Base(PColorSpace const& space, Dimension const& dim, FrameType type)
  : dim_( dim )
  , type_( type )
{
  space->CheckDim(dim, MaybeInterlaced(type));
}


void Base::SetType(FrameType type)
{
  GetColorSpace()->CheckDim(dim_, MaybeInterlaced(type));
  ClearStaticProperties();
  type_ = type;
}




void Base::SetProperty(CPProperty const& prop)
{
  if ( prop )
    if ( prop->IsStatic() )
      statics_[ prop->GetKey() ] = prop;
    else flows_.insert( std::make_pair(prop->GetKey(), prop) );
}


void Base::SetStaticProperty(CPProperty const& prop) const
{
  if ( prop && prop->IsStatic() )
    statics_[ prop->GetKey() ] = prop;
}


CPProperty Base::GetStaticProperty(PKey const& key) const
{
  PropertyMap::const_iterator it = statics_.find( key );
  return   it != statics_.end() ? it->second : CPProperty();
}

void Base::GetFlowProperties(PKey const& key, PropertyVector& propVector) const
{
  typedef PropertyMultiMap::const_iterator c_iter;
  std::pair<c_iter, c_iter> range = flows_.equal_range(key);

  std::transform(range.first, range.second, std::back_inserter(propVector), 
    boost::bind( &PropertyMultiMap::value_type::second, _1) );
}


void Base::RemoveFlowProperties(PKey const& key)
{
  flows_.erase(key);
}




} } } //namespace avs::vframe::concrete

