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


//avisynth include
#include "vframe_base.h"
#include "../colorspace.h"


namespace avs {




void VideoFrame::Base::SetProperty(CPProperty prop)
{
  Property::Key const& key = prop->GetKey();
  for( PropertyVector::iterator it = propVector.begin(); it != propVector.end(); ++it )    
    if ( (*it)->GetKey() == key )
    {
      *it = prop;
      return;
    }
  propVector.push_back(prop);  //case where we reach end without finding it
}


void VideoFrame::Base::RemoveProperty(Property::Key const& key)
{
  for( PropertyVector::iterator it = propVector.begin(); it != propVector.end(); ++it )
    if ( (*it)->GetKey() == key )
    {
      propVector.erase(it);
      return;
    }
}


CPProperty VideoFrame::Base::GetProperty(Property::Key const& key) const
{
  CPProperty result;
  for( PropertyVector::const_iterator it = propVector.begin(); it != propVector.end(); ++it )
    if ( (*it)->GetKey() == key )
    {
      result = *it;
      break;
    }
  return result;  
}



Dimension VideoFrame::Base::ToPlaneDimension(ColorSpace& space, Dimension const& dim, Plane plane)
{
  int x = dim.GetWidth();
  int y = dim.GetHeight();

  space.ToPlaneCoordinates(x, y, plane);

  return Dimension(x, y);
}


void VideoFrame::Base::CheckCompatible(ColorSpace& space, Dimension const& dim, Type type)
{
  space.CheckCoordinates( dim.GetWidth(), dim.GetHeight(), MaybeInterlaced(type) );
}


} //namespace avs

