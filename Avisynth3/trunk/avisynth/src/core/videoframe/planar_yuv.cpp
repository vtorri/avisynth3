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
#include "planar_yuv.h"


namespace avs { namespace vframe {


void PlanarYUV::ChangeSize(Vecteur const& topLeft, Vecteur const& bottomRight)
{
  ColorSpace & space = GetColorSpace();

  //verify params validity
  space.CheckVect(topLeft, MaybeInterlaced());
  space.CheckVect(bottomRight, MaybeInterlaced());

  //make changes
  GetY().ChangeSize( space.ToPlaneVect(topLeft, PLANAR_Y), space.ToPlaneVect(bottomRight, PLANAR_Y) );
  GetU().ChangeSize( space.ToPlaneVect(topLeft, PLANAR_U), space.ToPlaneVect(bottomRight, PLANAR_U) );
  GetV().ChangeSize( space.ToPlaneVect(topLeft, PLANAR_V), space.ToPlaneVect(bottomRight, PLANAR_V) );
  SetDimension( Dimension(GetDimension()) += bottomRight - topLeft );
  //NB: space.Check test skipped, we know dim is ok since bottomRight and topLeft are
}


void PlanarYUV::Copy(VideoFrame const& other, Vecteur const& coords)
{
  ColorSpace& space = GetColorSpace();

  if ( other.GetColorSpace() == space )
  {
    space.CheckVect(coords, MaybeInterlaced());
    
    GetY().Copy( other[PLANAR_Y], space.ToPlaneVect(coords, PLANAR_Y) );
    GetU().Copy( other[PLANAR_U], space.ToPlaneVect(coords, PLANAR_U) );
    GetV().Copy( other[PLANAR_V], space.ToPlaneVect(coords, PLANAR_V) );
  }
}






} } //namespace avs::vframe